﻿#pragma once

#include <array>
#include <vector>
#include <sstream>
#include "AES.h"
#include "Struct.h"

/**
 * 从流中读取一些数据
 */
template <int _Value, typename _Stream>
static std::array<char, _Value> ReadSome(_Stream &stream)
{
	std::array<char, _Value> buffer;
	for (unsigned int i = 0; i < _Value; ++i) buffer[i] = stream.get();
	return buffer;
}

/**
 * 从流中读取大量数据
 */
template <typename _Stream>
static std::stringstream ReadLarge(_Stream &stream, const int readsize)
{
	std::stringstream ss;
	for (int i = 0; i < readsize; ++i) ss.put(stream.get());
	return ss;
}

/**
 * 将数据写到流中
 */
template <typename _Stream>
static void WriteToSteam(const void *data, uint32_t size, _Stream &stream)
{
	unsigned char *p = reinterpret_cast<unsigned char *>(const_cast<void *>(data));
	for (unsigned int i = 0; i < size; ++i) stream.put(p[i]);
}

/**
 * 移动流数据到另一个流
 */
template <typename _Source, typename _Target>
static void StreamMove(_Target &target, _Source &source, const uint32_t size)
{
	for (uint64_t i = 0; i < size; ++i) target.put(source.get());
}

/**
 * 数据块加密
 */
static void EncryptBlock(std::stringstream &ss, const aes_key &key)
{
	const std::streamoff contents_size = ss.tellp() - ss.tellg();
	const uint32_t block_size = (uint32_t)(contents_size + AES_BLOCK_SIZE - contents_size % AES_BLOCK_SIZE);
	std::vector<uint8_t> buffer;
	buffer.resize(block_size);
	for (uint32_t i = 0; i < contents_size; ++i) buffer[i] = ss.get();
	AES::EncryptData(&buffer[0], block_size, key);
	ss.seekg(0); ss.seekp(0);
	for (uint32_t i = 0; i < block_size; ++i) ss.put(buffer[i]);
}

/**
 * 数据块解密
 */
static void DecryptBlock(std::stringstream &ss, const aes_key &key)
{
	const std::streamoff contents_size = ss.tellp() - ss.tellg();
	const uint32_t block_size = (uint32_t)(contents_size + AES_BLOCK_SIZE - contents_size % AES_BLOCK_SIZE);
	std::vector<uint8_t> buffer;
	buffer.resize(block_size);
	for (uint32_t i = 0; i < contents_size; ++i) buffer[i] = ss.get();
	AES::DecryptData(&buffer[0], block_size, key);
	ss.seekg(0); ss.seekp(0);
	for (uint32_t i = 0; i < block_size; ++i) ss.put(buffer[i]);
}