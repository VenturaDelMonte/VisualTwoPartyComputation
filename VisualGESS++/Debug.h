#pragma once

void				InitLogger();

namespace detail
{
void	__cdecl		Log_impl(const char *fmt, ...);
void	__cdecl		Error_impl(const char *fmt, ...) THROWS_EXCEPTION;
};

#define Log detail::Log_impl
#define Error detail::Error_impl