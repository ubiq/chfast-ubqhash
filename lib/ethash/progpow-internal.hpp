// ethash: C/C++ implementation of Ethash, the Ethereum Proof of Work algorithm.
// Copyright 2018 Pawel Bylica.
// Licensed under the Apache License, Version 2.0. See the LICENSE file.

#pragma once

#include "kiss99.h"
#include <ethash/ethash.hpp>
#include <array>

namespace progpow
{
using namespace ethash;

static constexpr int num_regs = 16;
static constexpr size_t l1_cache_size = 16 * 1024;
static constexpr size_t l1_cache_num_items = l1_cache_size / sizeof(uint32_t);

/// A variant of Keccak hash function for ProgPoW.
///
/// This Keccak hash function uses 800-bit permutation (Keccak-f[800]) with 576 bitrate.
/// It take exactly 576 bits of input (split across 3 arguments) and adds no padding.
///
/// @param header_hash  The 256-bit header hash.
/// @param nonce        The 64-bit nonce.
/// @param mix_hash     Additional 256-bits of data.
/// @return             The 256-bit output of the hash function.
hash256 keccak_progpow_256(
    const hash256& header_hash, uint64_t nonce, const hash256& mix_hash) noexcept;

/// The same as keccak_progpow_256() but returns only 64-bit output.
uint64_t keccak_progpow_64(
    const hash256& header_hash, uint64_t nonce, const hash256& mix_hash) noexcept;


struct mix_state
{
    kiss99_state rng_state;
    std::array<uint32_t, num_regs> index_sequence;  // TODO: Try size_t and uint8_t.
};

mix_state init(uint64_t seed) noexcept;

uint32_t random_math(uint32_t a, uint32_t b, uint32_t selector) noexcept;

void random_merge(uint32_t& a, uint32_t b, uint32_t selector) noexcept;

void build_l1_cache(uint32_t cache[l1_cache_num_items], const epoch_context& context) noexcept;

}  // namespace progpow
