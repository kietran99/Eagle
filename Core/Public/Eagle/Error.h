#pragma once

#include <variant>

namespace eagle
{
	enum class ErrorKind
	{
		Os,
		InvalidBufferSize,
		Other,
	};

	struct ErrorDataOs
	{
		unsigned long Code;
		std::string Message;
	};

	struct ErrorDataSimple
	{
		ErrorKind Kind;
	};

	struct ErrorDataSimpleMessage
	{
		ErrorKind Kind;
		std::string Str;
	};

	using ErrorData = std::variant<
		ErrorDataOs,
		ErrorDataSimple,
		ErrorDataSimpleMessage
	>;

	const char* ToStr(ErrorKind kind);

	struct Error
	{
		ErrorData Data;

		static Error New(ErrorKind kind);
		static Error New(ErrorKind kind, const std::string& payload);
		static Error LastOsError();

		ErrorKind Kind() const;
		const char* Message() const;
	};
}
