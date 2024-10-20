#pragma once

namespace eagle
{
	struct ErrorGeneric
	{
		ErrorGeneric(const std::string& msg)
			: Message(msg)
		{}

		std::string Message;
	};

	ErrorGeneric MakeLastError();
}