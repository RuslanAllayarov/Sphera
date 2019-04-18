#pragma once
#include <iostream>
#include <string>

enum class Error
{
    NoError,
    CorruptedArchive
};

class Serializer
{
    static constexpr char Separator = ' ';
    std::ostream& out_;
public:
    explicit Serializer(std::ostream& out)
        : out_(out)
    {
    }
    template <class T>
    Error save(T& object)
    {
        return object.serialize(*this);
    }
    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }
private:
    template <class T, class... ArgsT>
    Error process(T && value, ArgsT && ... args) 
    {
        if (process(value) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);

        return Error::CorruptedArchive;
    }    
    Error process(bool& value)
    {
        std::string text;
        if (value)
            text = "true";
        else
            text = "false";
        if (out_ << text << Separator)
            return Error::NoError;
        return Error::CorruptedArchive;
    }

    Error process(uint64_t& n)
    {
        if (out_ << n << Separator)
            return Error::NoError;
        return Error::CorruptedArchive;
    }
};

class Deserializer
{
    std::istream& in_;
public:
    explicit Deserializer(std::istream& in)
        : in_(in)
    {
    }

    template <class T>
    Error load(T& object)
    {
        return object.serialize(*this);
    }

    template <class... Args>
    Error operator()(Args&&... args)
    {
        return process(std::forward<Args>(args)...);
    }

private:

    template <class T, class... ArgsT>
    Error process(T && value, ArgsT && ... args) {
        if (process(value) == Error::NoError)
            return process(std::forward<ArgsT>(args)...);
        return Error::CorruptedArchive;
    }    
    Error process(bool& value)
    {
        std::string text;
        in_ >> text;

        if (text == "true")
            value = true;
        else if (text == "false")
            value = false;
        else
            return Error::CorruptedArchive;

        return Error::NoError;
    }
    
    Error process(uint64_t& value)
    {
        std::string text;
        in_ >> text;
        if (isdigit(text[0]))
        {
            value = stoul(text);
            return Error::NoError;
        }
        return Error::CorruptedArchive;
    }
};
