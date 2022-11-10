#include "logger_2.h"


namespace kctf {

void LoggerT::LoggerStreamT::set_parentheses(const std::vector<ParenthesisT> &parentheses) {
    parentheses_ = parentheses;
}

bool LoggerT::used_code_fully_mathches(const std::vector<CodeT> &codes, const std::vector<ParenthesisT> &parenthesis) const {
    if (used_codes_.size() < codes.size()) {
        return false;
    }

    for (size_t i = 0; i < codes.size(); ++i) {
        if (used_codes_[i].code_ != codes[i] || used_codes_[i].parentheses_ != parenthesis[i]) {
            return false;
        }
    }

    return true;
}

void LoggerT::print_aligned(const StringT &str, Align align, size_t width) {
    if (width < str.size()) {
        stream_ << str;
        return;
    }

    if (align == Align::middle) {
        size_t left_padding = (width - str.size()) / 2;
        size_t right_padding = width - str.size() - left_padding;
        stream_ << StringT(left_padding, ' ') << str << StringT(right_padding, ' ');
    } else if (align == Align::left) {
        stream_ << str << StringT(width - str.size(), ' ');
    } else if (align == Align::right) {
        stream_ << StringT(width - str.size(), ' ') << str;
    }
}

void LoggerT::print_code(
    size_t code_index,
    const CodeT &code,
    const ParenthesisT &parentheses,
    bool to_print_parentheses_anyway,
    bool to_print_codes_anyway
) {
    if (code_index >= used_codes_.size()) {
        used_codes_.resize(code_index + 1);
    }

    if (used_codes_[code_index].code_ != code || used_codes_[code_index].parentheses_ != parentheses) {
        used_codes_[code_index].code_        = code;
        used_codes_[code_index].parentheses_ = parentheses;

        used_codes_[code_index].max_code_length_     = std::max(used_codes_[code_index].max_code_length_,     code.size());
        used_codes_[code_index].max_par_length_left  = std::max(used_codes_[code_index].max_par_length_left,  parentheses.left_.size());
        used_codes_[code_index].max_par_length_right = std::max(used_codes_[code_index].max_par_length_right, parentheses.right_.size());

        print_aligned(parentheses.left_, Align::left, used_codes_[code_index].max_par_length_left);
        print_aligned(code, Align::middle, used_codes_[code_index].max_code_length_);
        print_aligned(parentheses.right_, Align::right, used_codes_[code_index].max_par_length_right);
    } else {
        if (to_print_parentheses_anyway) {
            print_aligned(parentheses.left_, Align::left, used_codes_[code_index].max_par_length_left);
            if (to_print_codes_anyway) {
                print_aligned(code, Align::middle, used_codes_[code_index].max_code_length_);
            } else {
                stream_ << std::string(code.size(), ' ');
            }
            print_aligned(parentheses.right_, Align::right, used_codes_[code_index].max_par_length_right);
        } else {
            stream_ << std::string(
                + used_codes_[code_index].max_par_length_left
                + used_codes_[code_index].max_code_length_
                + used_codes_[code_index].max_par_length_right,
                ' '
            );
        }
    }
}

void LoggerT::print(
        Level level,
        const std::vector<CodeT> &codes,
        const StringT &message,
        const StringT &end,
        const StringT &split,
        const std::vector<ParenthesisT> &parentheses,
        bool to_print_codes_anyway
) {
    if (level > log_level_) {
        return;
    }
    std::lock_guard<std::mutex> lock(mutex_);

    if (parentheses.size() == 0) {
        throw std::runtime_error("Parentheses must not be empty");
    }

    bool to_print_parentheses_anyway = !used_code_fully_mathches(codes, parentheses);
    for (size_t i = 0; i < codes.size(); ++i) {
        print_code(i, codes[i], parentheses[i % parentheses.size()], to_print_parentheses_anyway, to_print_codes_anyway);
    }

    for (size_t i = codes.size(); i < used_codes_.size(); ++i) {
        print_code(i, used_codes_[i].code_, used_codes_[i].parentheses_, false, to_print_codes_anyway);
    }

    stream_ << split;

    stream_ << message << end;
}

void LoggerT::set_log_level(Level level) {
    log_level_ = level;
}

LoggerT::LoggerT(OutputStreamT &stream, Level log_level)
    : stream_(stream)
    , log_level_(log_level)

    , error(*this, Level::error, {"!!!!"})
    , warning(*this, Level::warning, {"~~~~"})
    , info(*this, Level::info, {"info"})
    , debug(*this, Level::debug, {"debg"})
    , trace(*this, Level::trace, {"trce"})
{}

LoggerT::LoggerStreamT::LoggerStreamT(
    LoggerT &logger,
    Level log_level,
    const std::vector<CodeT> &codes,
    const std::vector<ParenthesisT> &parentheses,
    const StringT &split,
    const StringT &end,
    bool to_print_codes_anyway
)
    : logger_(logger)
    , log_level_(log_level)
    , codes_(std::move(codes))
    , parentheses_(parentheses)
    , split_(split)
    , end_(end)
    , to_print_codes_anyway_(to_print_codes_anyway)
{}

LoggerT::LoggerStreamT::AdditionalCodeProxyT LoggerT::LoggerStreamT::operator()(const CodeT &code) {
    return AdditionalCodeProxyT{*this, std::vector<CodeT>{code}};
}

LoggerT::LoggerStreamT::AdditionalCodeProxyT LoggerT::LoggerStreamT::AdditionalCodeProxyT::operator()(const CodeT &code) {
    additional_codes_.push_back(code);
    return *this;
}

LoggerT::LoggerStreamT::AdditionalCodeProxyT::AdditionalCodeProxyT(LoggerStreamT &logger_stream, const std::vector<CodeT> &additional_codes)
    : logger_stream_(logger_stream)
    , additional_codes_(additional_codes)
{}

LoggerT::LoggerStreamT::ProxyT::ProxyT(LoggerStreamT &logger_stream, const std::vector<CodeT> &additional_codes)
    : logger_stream_(logger_stream)
    , ss_()
    , additional_codes_(additional_codes)
{}

LoggerT::LoggerStreamT::ProxyT::~ProxyT() {
    auto codes = logger_stream_.codes_;
    codes.insert(codes.end(), additional_codes_.begin(), additional_codes_.end());

    logger_stream_.logger_.print(
        logger_stream_.log_level_,
        codes,
        ss_.str(),
        logger_stream_.end_,
        logger_stream_.split_,
        logger_stream_.parentheses_,
        logger_stream_.to_print_codes_anyway_
    );
}

LoggerT::LoggerStreamT::ProxyT::ProxyT(ProxyT &other)
    : logger_stream_(other.logger_stream_)
    , ss_(std::move(other.ss_))
{}

LoggerT::LoggerStreamT::ProxyT::ProxyT(ProxyT &&other)
    : logger_stream_(other.logger_stream_)
    , ss_(std::move(other.ss_))
{}

const std::vector<LoggerT::ParenthesisT> LoggerT::default_parentheses_ {
    {"[", "]"},
    {"<", ">"},
    {"{", "}"},
    {"(", ")"}
};

} // namespace kctf
