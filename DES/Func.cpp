#include "Func.h"

string Encrypt(string data, string key)
{
	//data補滿 64bits * n
	if (data.size() % 8 != 0)
		data.append(string(8 - data.size() % 8, '='));

	//先把字串讀成c的字串；用uint64_t的指標去讀他的bit pattern
	char * str1 = (char*)malloc(sizeof(char)*data.size());
	strcpy(str1, data.c_str());
	cout << str1;
	std::vector<std::bitset<64>> str2; // !! **重要用到的** !!
	while (*str1 != NULL)
	{	//little-endian process
		char temp[8];
		temp[0] = str1[7];	temp[1] = str1[6];	temp[2] = str1[5];	temp[3] = str1[4];
		temp[4] = str1[3];	temp[5] = str1[2];	temp[6] = str1[1];	temp[7] = str1[0];
		str2.push_back(std::bitset<64>(*((uint64_t*)temp)));
		str1 += 8;
	}	

	//讀key的bit pattern (key是16進制的字串)
	if (key.size() != 8)
		throw std::length_error("key must 64-bit string.");
	string keyStr;
	for (int i = 0; i != key.size(); ++i)
	{
		switch (key.at(i))
		{
		case '0': keyStr.append("0000"); break;
		case '1': keyStr.append("0001"); break;
		case '2': keyStr.append("0010"); break;
		case '3': keyStr.append("0011"); break;
		case '4': keyStr.append("0100"); break;
		case '5': keyStr.append("0101"); break;
		case '6': keyStr.append("0110"); break;
		case '7': keyStr.append("0111"); break;
		case '8': keyStr.append("1000"); break;
		case '9': keyStr.append("1001"); break;
		case 'A': keyStr.append("1010"); break;
		case 'B': keyStr.append("1011"); break;
		case 'C': keyStr.append("1100"); break;
		case 'D': keyStr.append("1101"); break;
		case 'E': keyStr.append("1110"); break;
		case 'F': keyStr.append("1111"); break;
		default:
			throw std::domain_error("key is not hexadecimal.");	break;
		}
	}
	std::bitset<64> key2(keyStr); // !! **重要用到的** !!

	//對每個block作操作
	const int numOfBlocks = str2.size(); //算出總共有幾個blocks
	for (int i = 0; i != numOfBlocks; ++i)
	{
		std::bitset<64> blockData; (str2[i]);
	}

	return string("test1");
}

string Decrypt(string data, string key)
{
	return string("test2");
}
//
//std::string ProductEncrypt(std::string Plaintext, std::string key)
//{
//	//將key以空白分割為int vector
//	std::vector<int> keys = Split(key);
//
//	//不能整除 補空白
//	if (Plaintext.size() % keys.size() != 0)
//	{
//		int appendCount = keys.size() - (Plaintext.size() % keys.size());
//		for (size_t i = 0; i != appendCount; i++)
//			Plaintext.append(" ");
//	}
//
//	//如果key比明文短 則把key加長
//	int blockSize = keys.size();
//	std::vector<int> appendKeys(keys.size(), 0);
//	for (size_t i = 1; Plaintext.size() != keys.size(); ++i) //i=加長第幾個block
//	{
//		for (size_t j = 0; j != blockSize; j++) //把key加長
//		{
//			keys.push_back(keys.at(j) + blockSize * i);
//		}
//	}
//
//	//加密
//	string Ciphertext(Plaintext);
//	for (size_t i = 0; i < Ciphertext.size(); i++)
//	{
//		Ciphertext.at(i) = Plaintext.at(keys.at(i));
//	}
//
//	return  Ciphertext;
//}

/*
std::string ProductDecrypt(std::string Ciphertext, std::string key)
{
	//將key以空白分割為int vector
	std::vector<int> keys = Split(key);

	//如果key比明文短 則把key加長
	int blockSize = keys.size();
	std::vector<int> appendKeys(keys.size(), 0);
	for (size_t i = 1; Ciphertext.size() != keys.size(); ++i) //i=加長第幾個block
	{
		for (size_t j = 0; j != blockSize; j++) //把key加長
		{
			keys.push_back(keys.at(j) + blockSize * i);
		}
	}

	//解密
	string Plaintext(Ciphertext);
	for (size_t i = 0; i < Ciphertext.size(); i++)
	{
		Plaintext.at(keys.at(i)) = Ciphertext.at(i);
	}

	return Plaintext;
}
*/