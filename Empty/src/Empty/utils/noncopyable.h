#pragma once

namespace Empty::utils
{
    struct noncopyable
    {
        noncopyable() = default;
        noncopyable(noncopyable&&) = delete;
        noncopyable(const noncopyable&) = delete;
        noncopyable& operator=(const noncopyable&) = delete;
        noncopyable& operator=(noncopyable&&) = delete;
    };
}