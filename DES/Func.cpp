#include "Func.h"

string Encrypt(string data, string key)
{
	//data補滿 64bits * n
	if (data.size() % 16 != 0)
		data.append(string(16 - data.size() % 16, '0'));

	//data讀成bit pattern
	std::vector<std::bitset<64>> strBlocks; // !! **重要用到的變數** !!
	for (size_t i = 0; i < data.size(); i += 16) //每64bit讀一次 (16*4=64)
	{
		std::string block64temp;
		for (size_t j = 0; j < 16; j++) //每個字符，轉成binary
		{
			switch (data.at(i + j))
			{
			case '0': block64temp.append("0000"); break;
			case '1': block64temp.append("0001"); break;
			case '2': block64temp.append("0010"); break;
			case '3': block64temp.append("0011"); break;
			case '4': block64temp.append("0100"); break;
			case '5': block64temp.append("0101"); break;
			case '6': block64temp.append("0110"); break;
			case '7': block64temp.append("0111"); break;
			case '8': block64temp.append("1000"); break;
			case '9': block64temp.append("1001"); break;
			case 'A': block64temp.append("1010"); break;
			case 'B': block64temp.append("1011"); break;
			case 'C': block64temp.append("1100"); break;
			case 'D': block64temp.append("1101"); break;
			case 'E': block64temp.append("1110"); break;
			case 'F': block64temp.append("1111"); break;
			default:
				throw std::domain_error("data must be hexadecimal.");	break;
			}
		}
		strBlocks.push_back(std::bitset<64>(block64temp));
	}

	//讀key的bit pattern (key是16進制的字串)
	if (key.size() != 16) //金鑰長度必須為64-bits
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
			throw std::domain_error("key must be hexadecimal.");	break;
		}
	}
	std::bitset<64> key2(keyStr); // !! **重要用到的變數** !!

	//現在{ strBlocks是data(已經每個blocks分開); key2是key的bit pattern; }
	//以下對每個block進行DES操作

	//key schedule
	std::bitset<56> key56 = PC_1(key2); //Permuted choice 1
	std::string key56str = key56.to_string(); //key切分成左右兩半
	std::bitset<28> key28L(string(key56str.begin(), key56str.begin() + 28)); //左半
	std::bitset<28> key28R(string(key56str.begin() + 28, key56str.end())); //右半
	//std::bitset<28> key28L(key56str.substr(0, 28)); //左半
	//std::bitset<28> key28R(key56str.substr(28, 56)); //右半
	//_key schedule

	const int numOfBlocks = strBlocks.size(); //算出總共有幾個blocks, 每64bits作加密一次
	for (int i = 0; i != numOfBlocks; ++i)
	{
		//initial permutation
		strBlocks.at(i) = IP(strBlocks.at(i));

		// Feistel cipher * 16 Round 
		for (int round = 1; round != 17; ++round) //1~16round
		{
			//key schedule
			if (round == 1 || round == 2 || round == 9 || round == 16)
				shiftLeft(key28L, 1), shiftLeft(key28R, 1);
			else
				shiftLeft(key28L, 2), shiftLeft(key28R, 2);

			std::bitset<48> key48 = //round key (48bits)
				PC_2(bitset<56>(key28L.to_string() + key28R.to_string())); //Permuted choice 2
			//_key schedule

			//round with F funciton
			std::string blockData(strBlocks.at(i).to_string()); //bitset<64>
			std::bitset<32> Li_(string(blockData.begin(), blockData.begin() + 32)); //L i-1 (上次的左半邊)
			std::bitset<32> Ri_(string(blockData.begin() + 32, blockData.end())); //R i-1 (上次的右半邊)
			std::bitset<32> Li = Ri_; // (這次的左半邊)
			std::bitset<32> Ri = Li_ ^ F(Ri_, key48); // (這次的右半邊)
			strBlocks.at(i) = std::bitset<64>(string(Li.to_string() + Ri.to_string())); //回存進去data
			//_round with F funciton
		}

		// 左右交換
		shiftLeft(strBlocks.at(i), 32);

		//final permutation
		strBlocks.at(i) = IP_1(strBlocks.at(i));
	}

	//全部block已做完DES，現在要輸出成文字
	std::string resultData; //存放所有結果的bit pattern
	std::string outputData; //輸出的回傳字串
	for (auto i : strBlocks)
		resultData.append(i.to_string());
	//輸出成hexadecimal
	for (size_t i = 0; i < resultData.size(); i += 4)
	{
		// 4bits 為一個hexadecimal字符
		std::string result4b;
		result4b.push_back(resultData.at(i));
		result4b.push_back(resultData.at(i + 1));
		result4b.push_back(resultData.at(i + 2));
		result4b.push_back(resultData.at(i + 3));
		// 建立輸出
		if (result4b == string("0000")) outputData.push_back('0');
		else if (result4b == string("0001")) outputData.push_back('1');
		else if (result4b == string("0010")) outputData.push_back('2');
		else if (result4b == string("0011")) outputData.push_back('3');
		else if (result4b == string("0100")) outputData.push_back('4');
		else if (result4b == string("0101")) outputData.push_back('5');
		else if (result4b == string("0110")) outputData.push_back('6');
		else if (result4b == string("0111")) outputData.push_back('7');
		else if (result4b == string("1000")) outputData.push_back('8');
		else if (result4b == string("1001")) outputData.push_back('9');
		else if (result4b == string("1010")) outputData.push_back('A');
		else if (result4b == string("1011")) outputData.push_back('B');
		else if (result4b == string("1100")) outputData.push_back('C');
		else if (result4b == string("1101")) outputData.push_back('D');
		else if (result4b == string("1110")) outputData.push_back('E');
		else if (result4b == string("1111")) outputData.push_back('F');
		else throw std::domain_error("Conversion to hexadecimal error.");
	}
	return outputData;
}

string Decrypt(string data, string key)
{
	return string("test2");
}

bitset<64> IP(const bitset<64> &input)
{
	int keys[64] = { 58,50,42,34,26,18,10,2,60,52,44,36,28,20,12,4,62,54,46,38,30,22,14,6,64,56,48,40,32,24,16,8,57,49,41,33,25,17,9,1,59,51,43,35,27,19,11,3,61,53,45,37,29,21,13,5,63,55,47,39,31,23,15,7 };

	bitset<64> output;
	for (size_t i = 0; i < output.size(); i++)
	{
		output[63 - i] = input[64 - keys[i]]; //注意bitset的順序和string相反
	}

	return  output;
}

bitset<64> IP_1(const bitset<64> &input)
{
	int keys[64] = { 40,8,48,16,56,24,64,32,39,7,47,15,55,23,63,31,38,6,46,14,54,22,62,30,37,5,45,13,53,21,61,29,36,4,44,12,52,20,60,28,35,3,43,11,51,19,59,27,34,2,42,10,50,18,58,26,33,1,41,9,49,17,57,25 };

	bitset<64> output;
	for (size_t i = 0; i < output.size(); i++)
	{
		output[63 - i] = input[64 - keys[i]]; //注意bitset的順序和string相反
	}

	return  output;
}

bitset<48> E(const bitset<32> &input)
{
	int keys[48] = { 32,1,2,3,4,5,4,5,6,7,8,9,8,9,10,11,12,13,12,13,14,15,16,17,16,17,18,19,20,21,20,21,22,23,24,25,24,25,26,27,28,29,28,29,30,31,32,1 };

	bitset<48> output;
	for (size_t i = 0; i < output.size(); i++)
	{
		output[47 - i] = input[32 - keys[i]]; //注意bitset的順序和string相反
	}

	return  output;
}

bitset<32> S_boxes(const bitset<48> &input)
{
	string inStr(input.to_string());
	int sbox[8][4][16] = { {{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},{0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},{4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},{15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
						   {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},{3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},{0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},{13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
						   {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},{13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},{13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},{1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
						   {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},{13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},{10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},{3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
						   {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},{14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},{4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},{11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
						   {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},{10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},{9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},{4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
						   {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},{13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},{1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},{6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
						   {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},{1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},{7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},{2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}} };
	vector<bitset<4>> outputs(8);

	//試試看用openMP下去平行化查表 不一定會對 玩玩看!!
#pragma omp parallel for 
	for (int i = 0; i < 8; i++) //第幾個box
	{
		const int base = i * 6;
		//算第幾列
		char outer2b[2] = { 0 };
		outer2b[0] = inStr[base + 0] - '0';
		outer2b[1] = inStr[base + 5] - '0';
		int outer2bb = outer2b[0] * 2 + outer2b[1] * 1;
		//算第幾行
		char inner4b[4] = { 0 };
		for (int i = 0; i != 4; ++i)
			inner4b[i] = inStr[base + i + 1] - '0'; // 1 2 3 4
		int inner4bb = inner4b[0] * 8 + inner4b[1] * 4 + inner4b[2] * 2 + inner4b[3] * 1;
		//
		outputs.at(i) = sbox[i][outer2bb][inner4bb];
	}
	//throw std::runtime_error("S_boxes query error.");
	return bitset<32>(outputs[0].to_string() + outputs[1].to_string() + outputs[2].to_string() + outputs[3].to_string() +
		outputs[4].to_string() + outputs[5].to_string() + outputs[6].to_string() + outputs[7].to_string());
}

bitset<32> P(const bitset<32> &input)
{
	int keys[32] = { 16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25 };

	bitset<32> output;
	for (size_t i = 0; i < output.size(); i++)
	{
		output[31 - i] = input[32 - keys[i]]; //注意bitset的順序和string相反
	}

	return  output;
}

bitset<56> PC_1(const bitset<64> &input)
{
	int keys[56] = { 57,49,41,33,25,17,9,1,58,50,42,34,26,18,10,2,59,51,43,35,27,19,11,3,60,52,44,36,63,55,47,39,31,23,15,7,62,54,46,38,30,22,14,6,61,53,45,37,29,21,13,5,28,20,12,4 };

	bitset<56> output;
	for (size_t i = 0; i < output.size(); i++)
	{
		output[55 - i] = input[64 - keys[i]]; //注意bitset的順序和string相反
	}

	return  output;
}

bitset<48> PC_2(const bitset<56> &input)
{
	int keys[48] = { 14,17,11,24,1,5,3,28,15,6,21,10,23,19,12,4,26,8,16,7,27,20,13,2,41,52,31,37,47,55,30,40,51,45,33,48,44,49,39,56,34,53,46,42,50,36,29,32 };

	bitset<48> output;
	for (size_t i = 0; i < output.size(); i++)
	{
		output[47 - i] = input[56 - keys[i]]; //注意bitset的順序和string相反
	}

	return  output;

}

bitset<32> F(const bitset<32> &Ri_, const bitset<48> &roundKey)
{
	//Expansion
	bitset<48> step1 = E(Ri_); // 32->48
	//XOR with round key
	bitset<48> step2 = step1 ^ roundKey;
	//S-box substitution
	bitset<32> step3 = S_boxes(step2);//48->32
	//permutation P
	return P(step3);
}



/*
std::string ProductEncrypt(std::string Plaintext, std::string key)
{
	//將key以空白分割為int vector
	std::vector<int> keys = Split(key);

	//不能整除 補空白
	if (Plaintext.size() % keys.size() != 0)
	{
		int appendCount = keys.size() - (Plaintext.size() % keys.size());
		for (size_t i = 0; i != appendCount; i++)
			Plaintext.append(" ");
	}

	//如果key比明文短 則把key加長
	int blockSize = keys.size();
	std::vector<int> appendKeys(keys.size(), 0);
	for (size_t i = 1; Plaintext.size() != keys.size(); ++i) //i=加長第幾個block
	{
		for (size_t j = 0; j != blockSize; j++) //把key加長
		{
			keys.push_back(keys.at(j) + blockSize * i);
		}
	}

	//加密
	string Ciphertext(Plaintext);
	for (size_t i = 0; i < Ciphertext.size(); i++)
	{
		Ciphertext.at(i) = Plaintext.at(keys.at(i));
	}

	return  Ciphertext;
}
*/

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
