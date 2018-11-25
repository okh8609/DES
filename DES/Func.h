#pragma once
#include <string>
#include <bitset>
#include <cstring>
#include <cstdint>
#include <stdexcept>
#include <vector>
#include <iostream>
#include <omp.h> //OpenMP

using namespace std;

string Encrypt(string data,string key);
string Decrypt(string data, string key);

bitset<64> IP(const bitset<64>& input); //Initial permutation (IP) [ 64-64 ]
bitset<64> IP_1(const bitset<64>& input); //IP^-1 [ 64-64 ]
bitset<48> E(const bitset<32>& input); //Expansion function [ 32-48 ]
bitset<32> S_boxes(const bitset<48>& input); //S-boxes [ 6-4 ]
bitset<32> P(const bitset<32>& input); //Permutation [ 32-32 ]
bitset<56> PC_1(const bitset<64>& input); //Permuted choice 1 [ 64-56 ]
bitset<48> PC_2(const bitset<56>& input); //Permuted choice 2 [ 56-48 ]
bitset<32> F(const bitset<32>& Ri_, const bitset<48>& roundKey); //F function [ E -> XOR -> S-Box -> P ]


/* Rotete 
	this function will change the original bitset.
	we may not rotate m > N.
*/
template <std::size_t N> inline void shiftLeft(std::bitset<N>& b, unsigned m) { b = b << m | b >> (N - m); }
template <std::size_t N> inline void shiftRight(std::bitset<N>& b, unsigned m) { b = b >> m | b << (N - m); }

/*
// 回傳bitset反轉
template <std::size_t N>
std::bitset<N> reverse(const std::bitset<N>& b)
{
	std::string temp(b.to_string());
	std::reverse(temp.begin(), temp.end());
	return std::bitset<N>(temp);
}
*/
