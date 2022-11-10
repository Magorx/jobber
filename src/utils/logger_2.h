#pragma once


#include <iostream>
#include <mutex>
#include <string>
#include <sstream>
#include <vector>


namespace kctf {

class LoggerT {
private:
    using OutputStreamT = std::ostream;
    using StringT = std::string;
    using CodeT = StringT;

public:
    enum class Level {
        none = 0,
        error = 2,
        warning = 4,
        info = 6,
        debug = 10,
        trace = 20
    };

    enum class Align {
        left = -1,
        middle = 0,
        right = 1
    };

    struct ParenthesisT {
        StringT left_;
        StringT right_;

        bool operator!=(const ParenthesisT &other) const {
            return left_ != other.left_ || right_ != other.right_;
        }
    };

    class LoggerStreamT {
    private:
        class ProxyT {
        private:
            LoggerStreamT &logger_stream_;
            std::stringstream ss_;

            std::vector<CodeT> additional_codes_;

        public:
            ProxyT(LoggerStreamT &logger_stream, const std::vector<CodeT> &additional_codes = {});
            ~ProxyT();

            ProxyT(ProxyT &);
            ProxyT(ProxyT &&);

            template <typename T>
            ProxyT &operator<<(const T &value) {
                ss_ << value;
                return *this;
            }
        };

        class AdditionalCodeProxyT {
        private:
            LoggerStreamT &logger_stream_;
            std::vector<CodeT> additional_codes_;

        public:
            AdditionalCodeProxyT(LoggerStreamT &logger_stream, const std::vector<CodeT> &additional_codes);
            AdditionalCodeProxyT operator()(const CodeT &code);

            template <typename T>
            ProxyT operator<<(const T &value) {
                ProxyT proxy(logger_stream_, additional_codes_);
                proxy << value;
                return proxy;
            }
        };

    private:
        LoggerT &logger_;
        Level log_level_;
        std::vector<CodeT> codes_;
        std::vector<ParenthesisT> parentheses_;
        StringT split_;
        StringT end_;
        bool to_print_codes_anyway_;

    public:
        LoggerStreamT(LoggerT &logger,
                      Level log_level,
                      const std::vector<CodeT> &codes,
                      const std::vector<ParenthesisT> &parentheses=LoggerT::default_parentheses_,
                      const StringT &split=" : ",
                      const StringT &end="\n",
                      bool to_print_codes_anyway=true);

        AdditionalCodeProxyT operator()(const CodeT &additional_code);

        template <typename T>
        ProxyT operator<<(const T &value) {
            ProxyT proxy(*this);
            proxy << value;
            return proxy;
        }

        void set_parentheses(const std::vector<ParenthesisT> &parentheses);
    };

private:
    std::mutex mutex_;
    
    OutputStreamT &stream_;
    Level log_level_;

public:
    const static std::vector<ParenthesisT> default_parentheses_;

private:
    struct CodeEntryT {
        CodeT code_;
        size_t max_code_length_;

        ParenthesisT parentheses_;
        size_t max_par_length_left;
        size_t max_par_length_right;
    };

    std::vector<CodeEntryT> used_codes_;

private:
    bool used_code_fully_mathches(const std::vector<CodeT> &codes, const std::vector<ParenthesisT> &parenthesis) const;
    void print_code(size_t code_index, const CodeT &code, const ParenthesisT &parentheses, bool to_print_parentheses_anyway=false, bool to_print_codes_anyway=false);
    void print_aligned(const StringT &str, Align align, size_t width);

public:
    void print(
        Level level,
        const std::vector<CodeT> &codes,
        const StringT &message,
        const StringT &end="\n",
        const StringT &split=" : ",
        const std::vector<ParenthesisT> &parentheses=default_parentheses_,
        bool to_print_codes_anyway=true
    );

    void set_log_level(Level level);

    LoggerStreamT error;
    LoggerStreamT warning;
    LoggerStreamT info;
    LoggerStreamT debug;
    LoggerStreamT trace;
    

public:
    LoggerT(OutputStreamT &stream, Level log_level=Level::info);
};

/*

logger << "Hello, world!" << std::endl;
logger.debug << "info";
logger.trace("code") << "info";
logger.info("code", "announcer") << "info"; 

*/

} // namespace kctf