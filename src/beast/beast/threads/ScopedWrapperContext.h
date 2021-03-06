//------------------------------------------------------------------------------
/*
    This file is part of Beast: https://github.com/vinniefalco/Beast
    Copyright 2013, Vinnie Falco <vinnie.falco@gmail.com>

    Permission to use, copy, modify, and/or distribute this software for any
    purpose  with  or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL ,  DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#ifndef BEAST_THREADS_WRAPSCOPED_H_INCLUDED
#define BEAST_THREADS_WRAPSCOPED_H_INCLUDED

namespace beast {

/** Wraps a function object so invocation happens during a scoped container lifetime. */
/** @{ */
namespace detail {

template <typename ScopedType, typename Context, typename Handler>
class ScopedWrapper
{
public:
    ScopedWrapper (Context const& context, Handler const& handler)
        : m_context (context)
        , m_handler (handler)
        { }

    void operator() ()
    {
        ScopedType const scope (m_context);
        m_handler();
    }

private:
    Context const& m_context;
    Handler m_handler;
};

}

//------------------------------------------------------------------------------

/** Helper to eliminate the template argument at call sites. */
template <typename Context, typename ScopedType>
class ScopedWrapperContext
{
public:
    typedef Context             context_type;
    typedef ScopedType          scoped_type;

    class Scope
    {
    public:
        explicit Scope (ScopedWrapperContext const& owner)
            : m_scope (owner.m_context)
            { }
    private:
        scoped_type m_scope;
    };

    ScopedWrapperContext ()
        { }

    template <typename Arg>
    explicit ScopedWrapperContext (Arg& arg)
        : m_context (arg)
        { }

    template <typename Handler>
    detail::ScopedWrapper <ScopedType, Context, Handler> wrap (
        Handler const& handler)
    {
        return detail::ScopedWrapper <ScopedType, Context, Handler> (
            m_context, handler);
    }

private:
    Context m_context;
};

}

#endif
