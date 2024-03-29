#pragma once

#include <filesystem>
#include <string_view>

class FsWatcher {
public:

	auto watch(std::string_view pathToWatch) -> void {
		path = pathToWatch;
		previous = read();
	}

	auto hasChanged() -> bool {
		auto current = read();
		if(current <= previous) {
			return false;
		}

		previous = current;
		return true;
	}

private:
	using Timestamp = std::filesystem::file_time_type;

	auto read() -> Timestamp {
		std::error_code ec;
		auto current = std::filesystem::last_write_time(path, ec);
		if(ec) {
			return Timestamp{};
		}
		return current;
	}

	std::string_view path;
	Timestamp previous;
};
