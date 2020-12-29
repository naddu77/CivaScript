#include <iostream>
#include <string>
#include <concepts>
#include <vector>
#include <sstream>
#include <iomanip>
#include <vector>
#include <stack>
#include <algorithm>
#include <ranges>
#include <type_traits>

using namespace std;
using namespace std::string_literals;

template <typename T>
concept arithmetic = std::is_arithmetic_v<T>;

class NullType
{

};

class ListType
{
public:
    template <typename T>
    bool operator==(T const&) const
    {
        return false;
    }

    bool operator!() const
    {
        return false;
    }
};

class ObjectType
{
public:
    template <typename T>
    bool operator==(T const&) const
    {
        return false;
    }

    bool operator!() const
    {
        return false;
    }
};

template <typename T = NullType>
class CivaScriptType
{
    template <typename U>
    friend class CivaScriptType;

    friend ostream& operator<<(ostream& os, CivaScriptType const& t) 
    {
        if constexpr (is_integral_v<T>)
        {
            if (static_cast<long long>(t.v) == 9999999999999999)
                os << 10000000000000000;
            else
                os << t.v;
        }
        else if constexpr (is_same_v<T, ListType>)
            os << quoted("");
        else if constexpr (is_same_v<T, ObjectType>)
            os << quoted("[object Object");
        else if constexpr (is_same_v<T, NullType>)
            return os;
        else if constexpr (is_convertible_v<T, string>)
            os << quoted(t.v);
        else
            os << t.v;

        return os;
    }

    template <typename T>
    friend auto operator+(CivaScriptType<T> const& t1, CivaScriptType<T> const& t2)->std::conditional_t<std::is_same_v<T, bool>, CivaScriptType<int>, CivaScriptType<T>>;

    template <typename T>
    friend auto operator-(CivaScriptType<T> const& t1, CivaScriptType<T> const& t2)->std::conditional_t<std::is_same_v<T, bool>, CivaScriptType<int>, CivaScriptType<T>>;

    template <convertible_to<string> T, arithmetic U>
    friend CivaScriptType<string> operator+(CivaScriptType<T> const& t, CivaScriptType<U> const& u);

    template <convertible_to<string> T, arithmetic U>
    friend CivaScriptType<U> operator-(CivaScriptType<T> const& t, CivaScriptType<U> const& u);

    friend CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<ObjectType>);
    friend CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<ListType>);

    template <convertible_to<string> U>
    friend CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<U> const& u);

    template <convertible_to<string> U>
    friend auto operator+(CivaScriptType<ObjectType>, CivaScriptType<U> const& u);

    template <arithmetic U>
    friend CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<U> const&);
    friend CivaScriptType<int> operator+(CivaScriptType<ObjectType>, CivaScriptType<ListType>);
    friend CivaScriptType<string> operator+(CivaScriptType<ObjectType>, CivaScriptType<ObjectType>);
    template <arithmetic U>
    friend CivaScriptType<string> operator+(CivaScriptType<ObjectType>, CivaScriptType<U> const& u);

    friend CivaScriptType<bool> operator+(CivaScriptType<NullType>, CivaScriptType<ListType>);

    template <typename T>
    friend CivaScriptType<T> operator-(CivaScriptType<T> const& t1, CivaScriptType<T> const& t2);

    friend CivaScriptType<int> operator-(CivaScriptType<bool> const& t1, CivaScriptType<bool> const& t2);
    friend CivaScriptType<string> operator-(CivaScriptType<ListType>, CivaScriptType<ObjectType>);
    friend CivaScriptType<int> operator-(CivaScriptType<ListType>, CivaScriptType<ListType>);

    template <convertible_to<string> U>
    friend auto operator-(CivaScriptType<ListType>, CivaScriptType<U> const& u);

    template <convertible_to<string> U>
    friend auto operator-(CivaScriptType<ObjectType>, CivaScriptType<U> const& u);

    template <arithmetic U>
    friend CivaScriptType<U> operator-(CivaScriptType<ListType>, CivaScriptType<U> const& u);

    friend CivaScriptType<int> operator-(CivaScriptType<ObjectType>, CivaScriptType<ListType>);
    friend CivaScriptType<string> operator-(CivaScriptType<ObjectType>, CivaScriptType<ObjectType>);

    template <arithmetic U>
    friend CivaScriptType<int> operator-(CivaScriptType<ObjectType>, CivaScriptType<U> const& u);

public:
    size_t length;

    CivaScriptType() = default;

    CivaScriptType(T const& v)
        : v(v), length(sizeof(v))
    {

    }

    CivaScriptType(T const& v, size_t length)
        : v(v), length(length)
    {

    }

    ~CivaScriptType() = default;

    template <typename U>
    bool operator==(CivaScriptType<U> const& u) const
    {
        if constexpr (is_same_v<T, U> || is_convertible_v<T, U> || is_convertible_v<U, T>)
            return static_cast<common_type_t<T, U>>(v) == static_cast<common_type_t<T, U>>(u.v);
        else if constexpr (is_same_v<T, ListType> && (is_integral_v<U> || is_floating_point_v<U>))
            return 0 == u.v;
        else
            return false;
    }

    template <arithmetic U>
    requires (arithmetic<T>)
    auto operator+(CivaScriptType<U> const& u) const
    {
        if constexpr (is_same_v<T, bool> && is_same_v<U, bool>)
            return CivaScriptType<long long>(static_cast<long long>(v) + static_cast<long long>(u.v));
        else if constexpr (is_same_v<T, bool>)
            return CivaScriptType<T>(static_cast<T>(v) + u.v);
        else if constexpr (is_same_v<U, bool>)
            return CivaScriptType<T>(v + static_cast<T>(u.v));
        else
            return CivaScriptType<common_type_t<T, U>>(v + u.v);
    }

    template <convertible_to<string> U>
    requires (arithmetic<T>)
    CivaScriptType<string> operator+(CivaScriptType<U> const& u) const
    {
        stringstream ss;

        ss.precision(17);
        ss << fixed;
        ss << boolalpha;

        ss << v << u.v;
        
        return CivaScriptType<string>(ss.str());
    }

    CivaScriptType<string> operator+(CivaScriptType<ListType>) const requires (!(is_same_v<T, ListType> || is_same_v<T, ObjectType>))
    {
        if constexpr (is_convertible_v<T, string>)
            return *this;
        else
        {
            stringstream ss;

            ss.precision(17);
            ss << fixed;
            ss << boolalpha;

            ss << v;

            return ss.str();
        }
    }

    CivaScriptType<string> operator+(CivaScriptType<ObjectType>) const requires (!(is_same_v<T, ListType> || is_same_v<T, ObjectType>))
    {
        if constexpr (is_convertible_v<T, string>)
            return v + "[object Object]";
        else
        {
            stringstream ss;

            ss.precision(17);
            ss << fixed;
            ss << boolalpha;

            ss << v;

            return ss.str() + "[object Object]";
        }
    }

    template <arithmetic U>
    requires (arithmetic<T>)
    auto operator-(CivaScriptType<U> const& u) const
    {
        if constexpr (is_same_v<T, bool> && is_same_v<U, bool>)
            return CivaScriptType<long long>(static_cast<long long>(v) - static_cast<long long>(u.v));
        else if constexpr (is_same_v<T, bool>)
            return CivaScriptType<T>(static_cast<T>(v) - u.v);
        else if constexpr (is_same_v<U, bool>)
            return CivaScriptType<T>(v - static_cast<T>(u.v));
        else
            return CivaScriptType<common_type_t<T, U>>(v - u.v);
    }

    CivaScriptType operator-(CivaScriptType<ListType>) const requires (!(is_same_v<T, ListType> || is_same_v<T, ObjectType>))
    {
        return *this;
    }

    CivaScriptType<string> operator-(CivaScriptType<ObjectType>) const requires (!(is_same_v<T, ListType> || is_same_v<T, ObjectType>))
    {
        return CivaScriptType<string>("NaN");
    }

    CivaScriptType<bool> operator!() const
    {
        if constexpr (is_same_v<T, ListType>)
            return !ListType{};
        else if constexpr (is_same_v<T, ObjectType>)
            return !ObjectType{};
        else if constexpr (is_convertible_v<T, string>)
            return empty(v);
        else
            return !v;
    }

    template <convertible_to<string> U>
    auto operator-(CivaScriptType<U> const& u) const
    {
        if constexpr (is_integral_v<T>)
            return CivaScriptType<T>(v - stoi(u.v));
        else if constexpr (is_floating_point_v<T>)
            return CivaScriptType<T>(v - stod(u.v));
        else
            return CivaScriptType<double>(stod(v) - stod(u.v));
    }

    static string_view max()
    {
        return "-Infinity";
    }

    static string_view min()
    {
        return "Infinity";
    }

    template <typename U>
    bool Equals(CivaScriptType<U> const& u) const
    {
        if constexpr (is_same_v<T, U>)
            return v == u.v;
        else
            return false;
    }

private:
    T v;
};

template <typename T>
auto operator+(CivaScriptType<T> const& t1, CivaScriptType<T> const& t2) -> std::conditional_t<std::is_same_v<T, bool>, CivaScriptType<int>, CivaScriptType<T>>
{
    if constexpr (std::is_same_v<T, bool>)
        return CivaScriptType<int>(static_cast<int>(t1.v) + static_cast<int>(t2.v));
    else
        return CivaScriptType<T>(t1.v + t2.v);
}

template <typename T>
auto operator-(CivaScriptType<T> const& t1, CivaScriptType<T> const& t2) -> std::conditional_t<std::is_same_v<T, bool>, CivaScriptType<int>, CivaScriptType<T>>
{
    if constexpr (std::is_same_v<T, bool>)
        return CivaScriptType<int>(static_cast<int>(t1.v) - static_cast<int>(t2.v));
    else
        return CivaScriptType<T>(t1.v - t2.v);
}

template <convertible_to<string> T, arithmetic U>
CivaScriptType<string> operator+(CivaScriptType<T> const& t, CivaScriptType<U> const& u)
{
    stringstream ss;

    ss.precision(17);
    ss << fixed;
    ss << boolalpha;

    ss << t.v << u.v;

    return CivaScriptType<string>(ss.str());
}

template <convertible_to<string> T, arithmetic U>
CivaScriptType<U> operator-(CivaScriptType<T> const& t, CivaScriptType<U> const& u)
{
    return CivaScriptType<U>(stod(t.v) - u.v);
}

CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<ObjectType>)
{
    return CivaScriptType<string>("[object Object]");
}

CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<ListType>)
{
    return CivaScriptType<string>("");
}

template <arithmetic U>
CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<U> const& u)
{
    return CivaScriptType<string>(to_string(u.v));
}

template <convertible_to<string> U>
CivaScriptType<string> operator+(CivaScriptType<ListType>, CivaScriptType<U> const& u)
{
    return CivaScriptType<string>(u.v);
}

template <convertible_to<string> U>
auto operator+(CivaScriptType<ObjectType>, CivaScriptType<U> const& u)
{
    if constexpr (is_integral_v<U>)
        return CivaScriptType<long long>(stoll(u.v));
    else if constexpr (is_floating_point_v<U>)
        return CivaScriptType<double>(stod(u.v));
    else
        return CivaScriptType<string>("NaN");
}

CivaScriptType<int> operator+(CivaScriptType<ObjectType>, CivaScriptType<ListType>)
{
    return CivaScriptType<int>(0);
}

CivaScriptType<string> operator+(CivaScriptType<ObjectType>, CivaScriptType<ObjectType>)
{
    return CivaScriptType<string>("[object Object][object Object]");
}

template <arithmetic U>
CivaScriptType<string> operator+(CivaScriptType<ObjectType>, CivaScriptType<U> const& u)
{
    return CivaScriptType<string>("[object Object]") + CivaScriptType<string>(to_string(u.v));
}

CivaScriptType<bool> operator+(CivaScriptType<NullType>, CivaScriptType<ListType>)
{
    return true;
}

template <typename T>
CivaScriptType<T> operator-(CivaScriptType<T> const& t1, CivaScriptType<T> const& t2)
{
    return t1.v - t2.v;
}

CivaScriptType<int> operator-(CivaScriptType<bool> const& t1, CivaScriptType<bool> const& t2)
{
    return CivaScriptType<int>(t1.v - t2.v);
}

CivaScriptType<string> operator-(CivaScriptType<ListType>, CivaScriptType<ObjectType>)
{
    return CivaScriptType<string>("NaN");
}

CivaScriptType<int> operator-(CivaScriptType<ListType>, CivaScriptType<ListType>)
{
    return CivaScriptType<int>(0);
}

template <convertible_to<string> U>
auto operator-(CivaScriptType<ListType>, CivaScriptType<U> const& u)
{
    if constexpr (is_integral_v<U>)
        return CivaScriptType<long long>(-stoll(u.v));
    else if constexpr (is_floating_point_v<U>)
        return CivaScriptType<double>(-stod(u.v));
    else
        return CivaScriptType<string>("NaN");
}

template <convertible_to<string> U>
auto operator-(CivaScriptType<ObjectType>, CivaScriptType<U> const& u)
{
    if constexpr (is_integral_v<U>)
        return CivaScriptType<long long>(-stoll(u.v));
    else if constexpr (is_floating_point_v<U>)
        return CivaScriptType<double>(-stod(u.v));
    else
        return CivaScriptType<string>("NaN");
}

template <arithmetic U>
CivaScriptType<U> operator-(CivaScriptType<ListType>, CivaScriptType<U> const& u)
{
    if constexpr (std::is_same_v<U, bool>)
        return CivaScriptType<U>(-static_cast<long long>(u.v));
    else
        return CivaScriptType<U>(-u.v);
}

CivaScriptType<int> operator-(CivaScriptType<ObjectType>, CivaScriptType<ListType>)
{
    return CivaScriptType(-0);
}

CivaScriptType<string> operator-(CivaScriptType<ObjectType>, CivaScriptType<ObjectType>)
{
    return CivaScriptType<string>("NaN");
}

template <arithmetic U>
CivaScriptType<int> operator-(CivaScriptType<ObjectType>, CivaScriptType<U> const& u)
{
    return CivaScriptType(-static_cast<int>(u.v));
}

//int main()
//{
//    cout << boolalpha;
//    cout << typeid(NAN).name() << endl;
//    cout << CivaScriptType(9999999999999999) << endl;
//    cout << (CivaScriptType<double>(0.5) + CivaScriptType<double>(0.1) == CivaScriptType<double>(0.6)) << endl;
//    cout << (CivaScriptType<double>(0.1) + CivaScriptType<double>(0.2) == CivaScriptType<double>(0.3)) << endl;
//    cout << CivaScriptType<>::max() << endl;
//    cout << CivaScriptType<>::min() << endl;
//    cout << CivaScriptType<ListType>() + CivaScriptType<ListType>() << endl;
//    cout << CivaScriptType<ListType>() + CivaScriptType<ObjectType>() << endl;
//    cout << CivaScriptType<ObjectType>() + CivaScriptType<ListType>() << endl;
//    cout << (CivaScriptType(true) + CivaScriptType(true) + CivaScriptType(true)).Equals(CivaScriptType(3)) << endl;
//    cout << CivaScriptType(true) - CivaScriptType(true) << endl;
//    cout << (CivaScriptType(true) == CivaScriptType(1)) << endl;
//    cout << CivaScriptType(true).Equals(CivaScriptType(1)) << endl;
//    cout << (CivaScriptType<>() + CivaScriptType<ListType>() + CivaScriptType<ListType>() + !CivaScriptType<ListType>()).length << endl;
//    cout << CivaScriptType<int>(9) + CivaScriptType<string>("1") << endl;
//    cout << CivaScriptType<int>(91) - CivaScriptType<string>("1") << endl;
//    cout << (CivaScriptType<ListType>() == CivaScriptType(0)) << endl;
//}

namespace CivaScript
{
    bool ContainsAlphabet(string_view token)
    {
        return ranges::any_of(token, [](auto c) { return isalpha(c); });
    }

    template <typename Func>
    string UnaryOp(string const& rhs, Func func)
    {
        stringstream ss;

        ss << boolalpha;

        if (rhs == "[]")
        {
            ss << invoke(func, CivaScriptType<ListType>());
        }
        else if (rhs == "{}")
        {
            ss << invoke(func, CivaScriptType<ObjectType>());
        }
        else if (rhs == "true" || rhs == "false")
        {
            ss << invoke(func, CivaScriptType(rhs == "true" ? true : false));
        }
        else if (rhs.find(".") != string_view::npos)
        {
            ss << invoke(func, CivaScriptType(stod(rhs)));
        }
        else if (rhs.starts_with("\"") && rhs.ends_with("\""))
        {
            ss << invoke(func, CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
        }
        else
        {
            ss << invoke(func, CivaScriptType(stoll(rhs)));
        }

        return ss.str();
    }

    template <typename Func>
    string BinaryOp(string const& original_lhs, string const& original_rhs, Func func)
    {
        stringstream ss;

        ss.precision(17);
        ss << fixed;
        ss << boolalpha;

        auto not_lhs{ original_lhs.find_first_not_of('!') };
        auto not_rhs{ original_rhs.find_first_not_of("!") };

        if (not_lhs == string::npos)
            not_lhs = size(original_lhs);

        if (not_rhs == string::npos)
            not_rhs = size(original_rhs);

        auto lhs{ original_lhs.substr(not_lhs) };
        auto rhs{ original_rhs.substr(not_rhs) };
        auto Invoke{ [not_lhs, not_rhs, &ss, func](auto&& t1, auto&& t2) {
            if (not_lhs % 2 == 0 && not_rhs % 2 == 0)
                ss << invoke(func, std::forward<decltype(t1)>(t1), std::forward<decltype(t2)>(t2));
            else if (not_lhs % 2 == 0)
                ss << invoke(func, std::forward<decltype(t1)>(t1), !std::forward<decltype(t2)>(t2));
            else if (not_rhs % 2 == 0)
                ss << invoke(func, !std::forward<decltype(t1)>(t1), std::forward<decltype(t2)>(t2));
            else
                ss << invoke(func, !std::forward<decltype(t1)>(t1), !std::forward<decltype(t2)>(t2));
        } };

        if (lhs == "[]")
        {
            if (rhs == "[]")
            {
                Invoke(CivaScriptType<ListType>(), CivaScriptType<ListType>());
            }
            else if (rhs == "{}")
            {
                Invoke(CivaScriptType<ListType>(), CivaScriptType<ObjectType>());
            }
            else if (rhs == "true" || rhs == "false")
            {
                Invoke(CivaScriptType<ListType>(), CivaScriptType(rhs == "true" ? true : false));
            }
            else if (rhs.find(".") != string_view::npos)
            {
                Invoke(CivaScriptType<ListType>(), CivaScriptType(stod(rhs)));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                Invoke(CivaScriptType<ListType>(), CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
            }
            else
            {
                Invoke(CivaScriptType<ListType>(), CivaScriptType(stoll(rhs)));
            }
        }
        else if (lhs == "{}")
        {
            if (rhs == "[]")
            {
                Invoke(CivaScriptType<ObjectType>(), CivaScriptType<ListType>());
            }
            else if (rhs == "{}")
            {
                Invoke(CivaScriptType<ObjectType>(), CivaScriptType<ObjectType>());
            }
            else if (rhs == "true" || rhs == "false")
            {
                Invoke(CivaScriptType<ObjectType>(), CivaScriptType(rhs == "true" ? true : false));
            }
            else if (rhs.find(".") != string_view::npos)
            {
                Invoke(CivaScriptType<ObjectType>(), CivaScriptType(stod(rhs)));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                Invoke(CivaScriptType<ObjectType>(), CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
            }
            else
            {
                Invoke(CivaScriptType<ObjectType>(), CivaScriptType(stoll(rhs)));
            }
        }
        else if (lhs == "true" || lhs == "false")
        {
            if (rhs == "[]")
            {
                Invoke(CivaScriptType(lhs == "true" ? true : false),  CivaScriptType<ListType>());
            }
            else if (rhs == "{}")
            {
                Invoke(CivaScriptType(lhs == "true" ? true : false), CivaScriptType<ObjectType>());
            }
            else if (rhs == "true" || rhs == "false")
            {
                Invoke(CivaScriptType(lhs == "true" ? true : false), CivaScriptType(rhs == "true" ? true : false));
            }
            else if (rhs.find(".") != string_view::npos)
            {
                Invoke(CivaScriptType(lhs == "true" ? true : false), CivaScriptType(stod(rhs)));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                Invoke(CivaScriptType(lhs == "true" ? true : false), CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
            }
            else
            {
                Invoke(CivaScriptType(lhs == "true" ? true : false), CivaScriptType(stoll(rhs)));
            }
        }
        else if (lhs.find(".") != string_view::npos)
        {
            if (rhs == "[]")
            {
                Invoke(CivaScriptType(stod(lhs)), CivaScriptType<ListType>());
            }
            else if (rhs == "{}")
            {
                Invoke(CivaScriptType(stod(lhs)), CivaScriptType<ObjectType>());
            }
            else if (rhs == "true" || rhs == "false")
            {
                Invoke(CivaScriptType(stod(lhs)), CivaScriptType(rhs == "true" ? true : false));
            }
            else if (rhs.find(".") != string_view::npos)
            {
                Invoke(CivaScriptType(stod(lhs)), CivaScriptType(stod(rhs)));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                Invoke(CivaScriptType(stod(lhs)), CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
            }
            else
            {
                Invoke(CivaScriptType(stod(lhs)), CivaScriptType(stod(rhs)));
            }
        }
        else if (lhs.starts_with("\"") && lhs.ends_with("\""))
        {
            auto lhs2{ lhs.substr(1, size(lhs) - 2) };

            if (rhs == "[]")
            {
                Invoke(CivaScriptType(lhs2, size(lhs2)), CivaScriptType<ListType>());
            }
            else if (rhs == "{}")
            {
                Invoke(CivaScriptType(lhs2, size(lhs2)), CivaScriptType<ObjectType>());
            }
            else if (rhs == "true" || rhs == "false")
            {
                Invoke(CivaScriptType(lhs2, size(lhs2)), CivaScriptType(rhs == "true" ? true : false));
            }
            else if (rhs.find(".") != string_view::npos)
            {
                Invoke(CivaScriptType(lhs2, size(lhs2)), CivaScriptType(stod(rhs)));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                Invoke(CivaScriptType(lhs2, size(lhs2)), CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
            }
            else
            {
                Invoke(CivaScriptType(lhs2, size(lhs2)), CivaScriptType(stod(rhs)));
            }
        }
        else if (empty(lhs))
        {
            if (rhs == "[]")
            {
                if (not_rhs < 2)
                {
                    if (not_lhs % 2 == 0)
                        ss << CivaScriptType(false);
                    else
                        ss << CivaScriptType(true);
                }
                else
                {
                    if ((not_lhs + not_rhs) % 2 == 0)
                        ss << CivaScriptType(true);
                    else
                        ss << CivaScriptType(false);
                }
            }
            else if (rhs == "{}")
            {
                if (not_rhs < 2)
                {
                    if (not_lhs % 2 == 0)
                        ss << CivaScriptType(false);
                    else
                        ss << CivaScriptType(true);
                }
                else
                {
                    if ((not_lhs + not_rhs) % 2 == 0)
                        ss << CivaScriptType(true);
                    else
                        ss << CivaScriptType(false);
                }
            }
            else if (rhs == "true" || rhs == "false")
            {
                if ((not_lhs + not_rhs) % 2 == 0)
                    ss << CivaScriptType(rhs == "true" ? true : false);
                else
                    ss << !CivaScriptType(rhs == "true" ? true : false);
            }
            else if (rhs.find(".") != string_view::npos)
            {
                if ((not_lhs + not_rhs) % 2 == 0)
                    ss << CivaScriptType(stod(rhs));
                else
                    ss << !CivaScriptType(stod(rhs));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                if ((not_lhs + not_rhs) % 2 == 0)
                    ss << CivaScriptType<string>(rhs.substr(1, size(rhs) - 2));
                else
                    ss << !CivaScriptType<string>(rhs.substr(1, size(rhs) - 2));
            }
            else
            {
                if ((not_lhs + not_rhs) % 2 == 0)
                    ss << CivaScriptType(stoll(rhs));
                else
                    ss << !CivaScriptType(stoll(rhs));
            }
        }
        else
        {
            if (rhs == "[]")
            {
                Invoke(CivaScriptType(stoll(lhs)), CivaScriptType<ListType>());
            }
            else if (rhs == "{}")
            {
                Invoke(CivaScriptType(stoll(lhs)), CivaScriptType<ObjectType>());
            }
            else if (rhs == "true" || rhs == "false")
            {
                Invoke(CivaScriptType(stoll(lhs)), CivaScriptType(rhs == "true" ? true : false));
            }
            else if (rhs.find(".") != string_view::npos)
            {
                Invoke(CivaScriptType(stoll(lhs)), CivaScriptType(stod(rhs)));
            }
            else if (rhs.starts_with("\"") && rhs.ends_with("\""))
            {
                Invoke(CivaScriptType(stoll(lhs)), CivaScriptType<string>(rhs.substr(1, size(rhs) - 2)));
            }
            else
            {
                Invoke(CivaScriptType(stoll(lhs)), CivaScriptType(stoll(rhs)));
            }
        }

        return ss.str();
    }

    string Eval(string_view line)
    {
        vector<string_view> tokens;
        auto prev{ 0 };

        for (auto i{ 0 }; i < ssize(line); ++i)
        {
            if ((i > 0 && isspace(line[i - 1]) && isspace(line[i])))
            {
                prev = i + 1;

                continue;
            }

            if (line[i] == '(' || line[i] == ')')
            {
                if (i - prev > 0)
                    tokens.emplace_back(&line[prev], i - prev);

                prev = i + 1;

                continue;
            }

            if (isspace(line[i]))
            {
                tokens.emplace_back(&line[prev], i - prev);
                prev = i + 1;
            }

            switch (line[i])
            {
            case '+':
                if (auto const len{ i - prev }; len > 0)
                    tokens.emplace_back(&line[prev], len);

                tokens.emplace_back(&line[i], 1);
                prev = i + 1;
                break;
            
            case '-':
                if (auto const len{ i - prev }; len > 0)
                    tokens.emplace_back(&line[prev], len);

                tokens.emplace_back(&line[i], 1);
                prev = i + 1;
                break;

            case '=':
            {
                if (auto const len{ i - prev }; len > 0)
                    tokens.emplace_back(&line[prev], len);

                auto temp{ i };

                while (line[i + 1] == '=')
                    ++i;

                tokens.emplace_back(&line[temp], i - temp + 1);

                prev = i + 1;
                break;
            }

            default:
                break;
            }
        }

        if (auto const len{ ssize(line) - prev }; len > 0)
            tokens.emplace_back(&line[prev], len);

        // For debugging
        //for (auto token : tokens)
        //    cout << token << endl;

        try
        {
            stack<string> st;
            stringstream ss;

            for (auto i{ 0 }; i < ssize(tokens); ++i)
            {
                if (tokens[i] == "typeof")
                {
                    if (tokens[i + 1] == "NaN")
                        ss << quoted("number");
                    else
                        ss << quoted("undefined");

                    break;
                }
                else if (tokens[i] == "Math.max")
                {
                    ss << CivaScriptType<>::max();

                    break;
                }
                else if (tokens[i] == "Math.min")
                {
                    ss << CivaScriptType<>::min();

                    break;
                }
                else if (tokens[i] == ".length")
                {
                    auto lhs{ st.top() };

                    st.pop();

                    if (lhs.starts_with("\"") && lhs.ends_with("\""))
                        lhs = lhs.substr(1, size(lhs) - 2);

                    ss << CivaScriptType<string>(lhs, size(lhs)).length;

                    break;
                }
                else if (tokens[i] == "+")
                {
                    auto lhs{ st.top() };

                    st.pop();

                    auto rhs{ string(tokens[i + 1]) };

                    st.push(BinaryOp(lhs, rhs, [](auto const& v1, auto const& v2) {
                        stringstream ss;

                        ss.precision(17);
                        ss << fixed;
                        ss << boolalpha;

                        ss << v1 + v2;

                        return ss.str();
                        }));

                    ++i;
                }
                else if (tokens[i] == "-")
                {
                    auto lhs{ st.top() };

                    st.pop();

                    auto rhs{ string(tokens[i + 1]) };

                    st.push(BinaryOp(lhs, rhs, [](auto const& v1, auto const& v2) {
                        stringstream ss;

                        ss.precision(17);
                        ss << fixed;
                        ss << boolalpha;
                        ss << v1 - v2;

                        return ss.str();
                        }));

                    ++i;
                }
                else if (tokens[i] == "==")
                {
                    auto lhs{ st.top() };

                    st.pop();

                    auto rhs{ string(tokens[i + 1]) };

                    st.push(BinaryOp(lhs, rhs, [](auto const& v1, auto const& v2) {
                        stringstream ss;

                        ss.precision(17);
                        ss << fixed;
                        ss << boolalpha;
                        ss << (v1 == v2);

                        return ss.str();
                        }));

                    ++i;
                }
                else if (tokens[i] == "===")
                {
                    auto lhs{ st.top() };

                    st.pop();

                    auto rhs{ string(tokens[i + 1]) };

                    st.push(BinaryOp(lhs, rhs, [](auto const& v1, auto const& v2) {
                        stringstream ss;

                        ss.precision(17);
                        ss << fixed;
                        ss << boolalpha;
                        ss << v1.Equals(v2);

                        return ss.str();
                        }));

                    ++i;
                }
                else
                    st.push(string(tokens[i]));
            }

            if (!empty(st))
            {
                if (auto const v{ st.top() }; ranges::all_of(v, [](auto c) { return isdigit(c); }))
                    ss << CivaScriptType<long long>(stoll(v));
                else
                    ss << v;
            }

            auto const result{ ss.str() };

            return empty(result) ? "undefined" : result;
        }
        catch (...)
        {
            return "undefined";
        }
    }
}

int main()
{
    stringstream ss;

    ss << "typeof NaN\n";
    ss << "9999999999999999\n";
    ss << "0.5+0.1==0.6\n";
    ss << "0.1+0.2==0.3\n";
    ss << "Math.max()\n";
    ss << "Math.min()\n";
    ss << "[]+[]\n";
    ss << "[]+{}\n";
    ss << "{}+[]\n";
    ss << "true+true+true===3\n";
    ss << "true-true\n";
    ss << "true==1\n";
    ss << "true===1\n";
    ss << "(!+[]+[]+![]).length\n";
    ss << "9+\"1\"\n";
    ss << "91-\"1\"\n";
    ss << "[]==0\n";

    for (string line; getline(ss, line); )
    {
        cout << "> " << line << '\n';
        cout << "< " << CivaScript::Eval(line) << std::endl;
    }

    cout << "\t\t\t\t\t\tThanks for inventing CivaScript" << endl;

    return 0;
}