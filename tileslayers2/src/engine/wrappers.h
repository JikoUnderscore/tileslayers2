#pragma once

#include <algorithm>
#include <vector>

template <class T>
struct Vec {
    std::vector<T> raw_vec;

    constexpr explicit Vec()noexcept: raw_vec(){}

    constexpr explicit Vec(const size_t count)noexcept : raw_vec(count) {
    }


    template <class... Valty>
    constexpr decltype(auto) emplace_back(Valty&&... _Val) noexcept {
        return this->raw_vec.emplace_back(std::forward<Valty>(_Val)...);
    }


    [[nodiscard]] constexpr decltype(auto) size() const noexcept {
        return this->raw_vec.size();
    }


    [[nodiscard]] constexpr decltype(auto) len() const noexcept {
        return this->raw_vec.size();
    }


    [[nodiscard]] constexpr T& operator[](const size_t pos) noexcept {
        return this->raw_vec[pos];
    }

    [[nodiscard]] constexpr T const& operator[](const size_t pos) const noexcept {
        return this->raw_vec[pos];
    }

    // vec must not be empty
    constexpr void swap_remove(const size_t index) noexcept {
        std::iter_swap(this->raw_vec.begin() + index, this->raw_vec.end() - 1);
        this->raw_vec.pop_back();
    }

    constexpr void push_back(T const& value) {
        this->raw_vec.push_back(value);
    }

    constexpr void push_back(T&& value) {
        this->raw_vec.push_back(std::move(value));
    }


    [[nodiscard]] constexpr decltype(auto) begin() noexcept {
        return this->raw_vec.begin();
    }

    [[nodiscard]] constexpr decltype(auto) begin() const noexcept {
        return this->raw_vec.begin();
    }


    [[nodiscard]] constexpr decltype(auto) end() noexcept {
        return this->raw_vec.end();
    }

    [[nodiscard]] constexpr decltype(auto) end() const noexcept {
        return this->raw_vec.end();
    }


    [[nodiscard]] constexpr decltype(auto) cbegin() const noexcept {
        return begin();
    }

    [[nodiscard]] constexpr decltype(auto) cend() const noexcept {
        return end();
    }

    [[nodiscard]] constexpr bool is_empty() const noexcept {
        return this->raw_vec.empty();
    }

    [[nodiscard]] constexpr bool empty() const noexcept {
        return this->raw_vec.empty();
    }

    constexpr void clear() noexcept {
        this->raw_vec.clear();
    }


    constexpr void reserve(const size_t new_capacity) {
        this->raw_vec.reserve(new_capacity);
    }
};
