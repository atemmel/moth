#include "args.hpp"
#include "common.hpp"

enum class Kind {
	Bool = 0,
	Str,
};

struct Value {
	Kind kind;
	void* ptr;
};

Vector<String> other;
Map<String, Value> flags;

auto flagImpl(void* value, Kind kind, StringView flag) {
	flags.insert({
		String(flag),
		Value{ .kind = kind, .ptr = value }, 
	});
}

auto flag(bool* value, StringView flag) {
	flagImpl(value, Kind::Bool, flag);
}

auto flag(String* value, StringView flag) {
	flagImpl(value, Kind::Str, flag);
}

auto hasFlag(const String& which) -> Value* {
	auto it = flags.find(which);
	if(it == flags.end()) {
		return nullptr;
	}
	return &(it->second);
}

auto parse(const Vector<String> &args) -> void {
	other.clear();
	for(Index i = 0; i < args.size(); i++) {
		const auto &arg = args[i];
		auto flag = has(flags, arg);
		if(flag == nullptr) {
			other.emplace_back(arg);
			continue;
		}
		switch(flag->kind) {
			case Kind::Bool:
				*as<bool>(flag->ptr) = true;
				break;
			case Kind::Str:
				unimplemented();
				break;
		}
	}
	flags.clear();
}

auto rest() -> const Vector<String>& {
	return other;
}
