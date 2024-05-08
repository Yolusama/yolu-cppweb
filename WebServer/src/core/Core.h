#pragma once
#include<memory>
#include<unordered_map>
#include<functional>
#include<nlohmann/json.hpp>

#define var auto

namespace CppWeb
{
	template<class T>
	using Ref = std::shared_ptr<T>;
	template<class T>
	using ScopeRef = std::unique_ptr<T>;
	template<typename K, typename V>
	using HashMap = std::unordered_map<K, V>;
	namespace CppJson = nlohmann;
}