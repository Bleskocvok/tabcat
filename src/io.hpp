#pragma once

#include <string>
#include <utility>
#include <fstream>
#include <iostream>


struct abs_input
{
    virtual ~abs_input() = default;

    virtual void open() {}
    virtual void close() {}

    virtual std::istream& istream() = 0;
};


struct abs_output
{
    virtual ~abs_output() = default;

    virtual void open() {}
    virtual void close() {}

    virtual std::ostream& ostream() = 0;
};


struct std_input : abs_input
{
    std::istream& istream() override { return std::cin; }
};


struct std_output : abs_output
{
    std::ostream& ostream() override { return std::cout; }
};


struct file_in : abs_input
{
    std::ifstream in;
    std::string filename;

    file_in(std::string filename)
        : filename(std::move(filename)) {}

    void open() override
    {
        in.open(filename.c_str());
        if (!in)
        {
            throw std::runtime_error("cannot open file '" + filename + "'");
        }
    }

    void close() override { in.close(); }

    std::istream& istream() override { return in; }
};


struct file_out : abs_output
{
    std::ofstream out;
    std::string filename;

    file_out(std::string filename)
        : filename(std::move(filename)) {}

    void open() override
    {
        out.open(filename.c_str());
        if (!out)
        {
            throw std::runtime_error("cannot open file '" + filename + "'");
        }
    }

    void close() override { out.close(); }

    std::ostream& ostream() override { return out; }
};
