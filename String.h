#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <math.h>

namespace AK {

	class String
	{
	protected:
		std::string str;
	public:

		inline static char None = '\0';

		String()
		{
			this->str = "";
		}

		String(bool actLikeFile, std::initializer_list<String> vals)
		{
            std::stringstream ss;

            if (actLikeFile)
            {
                for (auto elem : vals)
                {
                    ss << elem << '\n';
                }
            }
            else
            {
                for (auto elem : vals)
                {
                    ss << elem;
                }
            }

            this->str = ss.str();
		}

		template<typename T>
		String(const T val)
		{
			std::stringstream ss;
			ss << val;
			this->str = ss.str();
		}

		~String()
		{
		}

		size_t length()
		{
			return str.length();
		}

		size_t size()
		{
			return sizeof(str);
		}

		bool contains(String contain)
		{
			return (str.find(contain.str) != std::string::npos);
		}

		String substring(size_t start, size_t end)
		{
		    String ret;

            for (size_t i = start; i <= end; i++)
            {
                ret += str[i];
            }

			return ret;
		}

		static String format(String format, std::vector<String> args)
		{
			String fmt = format.copy();
			String formattedString;

			size_t i = 0, bracket_count = 0;

			bool useComments = true;

			try
			{
				while (i < fmt.length())
				{
				    if (fmt[i] == '%')
                    {
                        switch(fmt[i + 1])
                        {
                            case 't':
                            {
                                formattedString += __TIME__;
                                break;
                            }

                            case 'd':
                            {
                                formattedString += __DATE__;
                                break;
                            }

                            case 'b':
                            {
                                formattedString += bracket_count;
                                break;
                            }

                            case 'i':
                            {
                                formattedString += i;
                                break;
                            }

                            default:
                            {
                                throw(String(false,
                                             {
                                                "ERROR: ",
                                                "Failed to find the correct format when parsing \'%\' at index: (",
                                                i,
                                                ") ",
                                                "in format: \"",
                                                format,
                                                "\""
                                             }));
                            }
                        }

                        i++;
                    }
					else if (fmt[i] == '-')
					{
						i++;

						if (fmt[i] == 'c' && fmt[i + 1] == 'd')
						{
							useComments = false;
							i++;
						}
						/*else if (fmt[i] == 'c' && fmt[i + 1] == 'e')
						{
							useComments = true;
							i += 2;
						}*/
						else if (useComments == false)
						{
							formattedString += fmt[i - 1];
							formattedString += fmt[i];
						}

						if (useComments)
						{
							if (fmt[i] == '[')
							{
								size_t startIndex = i + 1;
								size_t testIndex = fmt.lastIndexOf("-[") + 2;
								size_t endIndex = fmt.lastIndexOf("]-") - 1;

								if (endIndex < 0 || testIndex < endIndex)
								{
								    String _throw = String(false, {"Error: ",
                                        "Did you forget to add a ending comment?\nin the string: ",
                                        "\"",
                                        format,
                                        "\""});
									throw(_throw);
								}

								fmt.replace(startIndex - 2, None);
								fmt.replace(startIndex - 1, None);

								fmt.replace(endIndex + 1, None);
								fmt.replace(endIndex + 2, None);

								i = endIndex + 1;

								formattedString += fmt.substring(startIndex, i - 5);
							}
							else
							{
								formattedString += fmt[i];
								fmt.replace(i - 1, None);
							}
						}
					}
					else if (fmt[i] != '{' && fmt[i] != '}')
					{
						formattedString += fmt[i];
					}
					else
					{
						size_t index = 0;
						size_t startIndex = 0;

						if (fmt[i] == '{')
						{
						    bracket_count++;
							startIndex = i + 1;
							String arg = None;

							fmt.replace(i, None);

							int safetyNet = 0;
							while (fmt[i] != '}' && safetyNet < 21)
							{
								i++;
								safetyNet++;
							}

							if (safetyNet >= 19)
							{
								String _throw = String(false, {"ERROR: ",
                                    "Did you forget an ending bracket when formatting the String: ",
                                    '\"',
                                    format,
                                    '\"',
                                    '?'});
								throw (_throw);
							}

							fmt.replace(i, None);

							arg = fmt.substring(startIndex, i - 1);

							if (arg == String('{') || arg == String('}'))
							{
								String _throw = String(false, {"ERROR: ",
                                    "No number was given inside formatting brakets!"});
								throw (_throw);
							}

							index = arg.convertToType<int>();
						}
						else if (fmt[i] == '}')
						{
							int safetyNet = 0;

							while (fmt[i] != '{' && safetyNet < 21)
							{
								i--;
								safetyNet++;
							}

							if (safetyNet >= 19)
							{
								String _throw = String(false, {"ERROR: ",
                                    "Did you forget an starting bracket when formatting the String: ",
									'\"',
									format,
									'\"',
									'?'});
								throw (_throw);
							}
						}

						if (index < args.size())
						{
							formattedString += args[index];
						}
						else
						{
							String _throw = String(false, {"ERROR: ",
								"Index given (",
								index,
								") is bigger than the given number of arguments!\nin string: ",
								'\"',
								format,
								'\"'});
							throw (_throw);
						}
					}

					i++;
				}

				formattedString = formattedString.removeAll(None);
			}
			catch (String err)
			{
				std::cout << err << std::endl;
				return "";
			}

			return formattedString;
		}

		static String DecToHex(unsigned long long num)
		{
		    String _num = String(num);
		    String ret;

		    unsigned long long holder1 = num % 16;
		    unsigned long long holder2 = num / 16;

		    while (holder2 >= 0)
            {
                switch (holder1)
                {
                    case 0: ret += "0"; break;
                    case 1: ret += "1"; break;
                    case 2: ret += "2"; break;
                    case 3: ret += "3"; break;
                    case 4: ret += "4"; break;
                    case 5: ret += "5"; break;
                    case 6: ret += "6"; break;
                    case 7: ret += "7"; break;
                    case 8: ret += "8"; break;
                    case 9: ret += "9"; break;
                    case 10: ret += "A"; break;
                    case 11: ret += "B"; break;
                    case 12: ret += "C"; break;
                    case 13: ret += "D"; break;
                    case 14: ret += "E"; break;
                    case 15: ret += "F"; break;
                }

                if (holder2 == 0) break;

                holder1 = holder2 % 16;
                holder2 = holder2 / 16;
            }

            return ret.flip();
		}

		static unsigned long long HexToDec(String num)
		{
		    String num_ = num.flip();
            unsigned long long ret = 0;
            unsigned long long number = 0;

            for (size_t i = 0; i < num_.length(); i++)
            {
                //std::cout << "\nnumber: " << num_[i];
                //std::cout << "\nindex: " << i;

                switch (num_[i])
                {
                    case '0': number = 0 * std::pow(16, i); break;
                    case '1': number = 1 * std::pow(16, i); break;
                    case '2': number = 2 * std::pow(16, i); break;
                    case '3': number = 3 * std::pow(16, i); break;
                    case '4': number = 4 * std::pow(16, i); break;
                    case '5': number = 5 * std::pow(16, i); break;
                    case '6': number = 6 * std::pow(16, i); break;
                    case '7': number = 7 * std::pow(16, i); break;
                    case '8': number = 8 * std::pow(16, i); break;
                    case '9': number = 9 * std::pow(16, i); break;
                    case 'A': number = 10 * std::pow(16, i); break;
                    case 'B': number = 11 * std::pow(16, i); break;
                    case 'C': number = 12 * std::pow(16, i); break;
                    case 'D': number = 13 * std::pow(16, i); break;
                    case 'E': number = 14 * std::pow(16, i); break;
                    case 'F': number = 15 * std::pow(16, i); break;
                }

                //std::cout << "\npower: " << number;
                ret += number;
            }
            //std::cout << "\n";
            return ret;
		}

		static String DecToBin(unsigned long long num)
		{
		    String _num = String(num);
		    String ret;

		    unsigned long long holder1 = num % 2;
		    unsigned long long holder2 = num / 2;

		    while (holder2 >= 0)
            {
                switch (holder1)
                {
                    case 0: ret += "0"; break;
                    case 1: ret += "1"; break;
                }

                if (holder2 == 0) break;

                holder1 = holder2 % 2;
                holder2 = holder2 / 2;
            }

            return ret.flip();
		}

		static unsigned long long BinToDec(String num)
		{
		    String num_ = num.flip();
            unsigned long long ret = 0;
            unsigned long long number = 0;

            for (size_t i = 0; i < num_.length(); i++)
            {
                //std::cout << "\nnumber: " << num_[i];
                //std::cout << "\nindex: " << i;

                switch (num_[i])
                {
                    case '0': number = 0 * std::pow(2, i); break;
                    case '1': number = 1 * std::pow(2, i); break;
                }

                //std::cout << "\npower: " << number;
                ret += number;
            }
            //std::cout << "\n";
            return ret;
		}

		static String HexToBin(String num)
		{
		    return DecToBin(HexToDec(num));
		}

		static String BinToHex(String num)
		{
		    return DecToHex(BinToDec(num));
		}

		static char get_base64_char(unsigned int num_holder)
        {
            char encoded_string;

            switch(num_holder)
            {
                case 0: encoded_string = 'A'; break;
                case 1: encoded_string = 'B'; break;
                case 2: encoded_string = 'C'; break;
                case 3: encoded_string = 'D'; break;
                case 4: encoded_string = 'E'; break;
                case 5: encoded_string = 'F'; break;
                case 6: encoded_string = 'G'; break;
                case 7: encoded_string = 'H'; break;
                case 8: encoded_string = 'I'; break;
                case 9: encoded_string = 'J'; break;
                case 10: encoded_string = 'K'; break;
                case 11: encoded_string = 'L'; break;
                case 12: encoded_string = 'M'; break;
                case 13: encoded_string = 'N'; break;
                case 14: encoded_string = 'O'; break;
                case 15: encoded_string = 'P'; break;
                case 16: encoded_string = 'Q'; break;
                case 17: encoded_string = 'R'; break;
                case 18: encoded_string = 'S'; break;
                case 19: encoded_string = 'T'; break;
                case 20: encoded_string = 'U'; break;
                case 21: encoded_string = 'V'; break;
                case 22: encoded_string = 'W'; break;
                case 23: encoded_string = 'X'; break;
                case 24: encoded_string = 'Y'; break;
                case 25: encoded_string = 'Z'; break;
                case 26: encoded_string = 'a'; break;
                case 27: encoded_string = 'b'; break;
                case 28: encoded_string = 'c'; break;
                case 29: encoded_string = 'd'; break;
                case 30: encoded_string = 'e'; break;
                case 31: encoded_string = 'f'; break;
                case 32: encoded_string = 'g'; break;
                case 33: encoded_string = 'h'; break;
                case 34: encoded_string = 'i'; break;
                case 35: encoded_string = 'j'; break;
                case 36: encoded_string = 'k'; break;
                case 37: encoded_string = 'l'; break;
                case 38: encoded_string = 'm'; break;
                case 39: encoded_string = 'n'; break;
                case 40: encoded_string = 'o'; break;
                case 41: encoded_string = 'p'; break;
                case 42: encoded_string = 'q'; break;
                case 43: encoded_string = 'r'; break;
                case 44: encoded_string = 's'; break;
                case 45: encoded_string = 't'; break;
                case 46: encoded_string = 'u'; break;
                case 47: encoded_string = 'v'; break;
                case 48: encoded_string = 'w'; break;
                case 49: encoded_string = 'x'; break;
                case 50: encoded_string = 'y'; break;
                case 51: encoded_string = 'z'; break;
                case 52: encoded_string = '0'; break;
                case 53: encoded_string = '1'; break;
                case 54: encoded_string = '2'; break;
                case 55: encoded_string = '3'; break;
                case 56: encoded_string = '4'; break;
                case 57: encoded_string = '5'; break;
                case 58: encoded_string = '6'; break;
                case 59: encoded_string = '7'; break;
                case 60: encoded_string = '8'; break;
                case 61: encoded_string = '9'; break;
                case 62: encoded_string = '+'; break;
                case 63: encoded_string = '/'; break;
            }

            return encoded_string;
        }

        static unsigned int get_base64_index(char c)
        {
            unsigned int index = 0;

            switch(c)
            {
                case 'A': index = 0; break;
                case 'B': index = 1; break;
                case 'C': index = 2; break;
                case 'D': index = 3; break;
                case 'E': index = 4; break;
                case 'F': index = 5; break;
                case 'G': index = 6; break;
                case 'H': index = 7; break;
                case 'I': index = 8; break;
                case 'J': index = 9; break;
                case 'K': index = 10; break;
                case 'L': index = 11; break;
                case 'M': index = 12; break;
                case 'N': index = 13; break;
                case 'O': index = 14; break;
                case 'P': index = 15; break;
                case 'Q': index = 16; break;
                case 'R': index = 17; break;
                case 'S': index = 18; break;
                case 'T': index = 19; break;
                case 'U': index = 20; break;
                case 'V': index = 21; break;
                case 'W': index = 22; break;
                case 'X': index = 23; break;
                case 'Y': index = 24; break;
                case 'Z': index = 25; break;
                case 'a': index = 26; break;
                case 'b': index = 27; break;
                case 'c': index = 28; break;
                case 'd': index = 29; break;
                case 'e': index = 30; break;
                case 'f': index = 31; break;
                case 'g': index = 32; break;
                case 'h': index = 33; break;
                case 'i': index = 34; break;
                case 'j': index = 35; break;
                case 'k': index = 36; break;
                case 'l': index = 37; break;
                case 'm': index = 38; break;
                case 'n': index = 39; break;
                case 'o': index = 40; break;
                case 'p': index = 41; break;
                case 'q': index = 42; break;
                case 'r': index = 43; break;
                case 's': index = 44; break;
                case 't': index = 45; break;
                case 'u': index = 46; break;
                case 'v': index = 47; break;
                case 'w': index = 48; break;
                case 'x': index = 49; break;
                case 'y': index = 50; break;
                case 'z': index = 51; break;
                case '0': index = 52; break;
                case '1': index = 53; break;
                case '2': index = 54; break;
                case '3': index = 55; break;
                case '4': index = 56; break;
                case '5': index = 57; break;
                case '6': index = 58; break;
                case '7': index = 59; break;
                case '8': index = 60; break;
                case '9': index = 61; break;
                case '+': index = 62; break;
                case '/': index = 63; break;
            }

            return index;
        }

        static String EncodeToBase64(String string)
        {
            String encoded_string;
            String bin;
            String holder, holder2, holder3;
            std::vector<String> bins, bins2;

            unsigned long long num_holder = 0;

            for (size_t i = 0; i < string.length(); i++)
            {
                bin = DecToBin((unsigned char) string[i]);
                bin = bin.flip();
                while (bin.length() < 8)
                {
                    bin += "0";
                }
                bin = bin.flip();

                holder2 += bin;
            }

            for (size_t i = 0; i < holder2.length(); i += 6)
            {
                bins2.push_back(String(false, {holder2.substring(i, i + 5)}));
            }

            if (holder2.length() % 6 > 0)
            {
                String str = bins2[bins2.size() - 1];

                str = str.removeAll(' ');
                while (str.length() < 6)
                {
                    str += "0";
                }

                holder3 = str;
                bins2[bins2.size() - 1] = str;
            }

            for (String str : bins2)
            {
                encoded_string += get_base64_char(String::BinToDec(str));
            }

            return encoded_string;
        }

        static String DecodeFromBase64(String string)
        {
            String encoded_string;
            String bin;
            String holder, holder2, holder3;
            std::vector<String> bins, bins2;

            unsigned long long num_holder = 0;

            for (size_t i = 0; i < string.length(); i++)
            {
                bin = DecToBin(get_base64_index(string[i]));
                bin = bin.flip();
                while (bin.length() < 6)
                {
                    bin += "0";
                }
                bin = bin.flip();
                bins.push_back(bin);
            }

            for (String str : bins)
            {
                holder2 += str;
            }

            for (size_t i = 0; i < holder2.length(); i += 8)
            {
                if (i + 7 > holder2.length())
                    break;
                bins2.push_back(String(false, {holder2.substring(i, i + 7)}));
            }

            for (String str : bins2)
            {
                encoded_string += (char) BinToDec(str);
            }

            return encoded_string;
        }

		size_t indexOf(String string)
		{
			if (str.find(string.str) != (size_t)-1)
				return str.find(string.str);
			return -1;
		}

		size_t lastIndexOf(String string)
		{
			if (string.length() <= length() && str.find_last_of(string.str[0]) != (size_t)-1 && str.find_last_of(string.str[string.length()] != (size_t)-1))
				return str.find_last_of(string.str[0]);
			return -1;
		}

		char charAt(size_t index)
		{
			return str[index];
		}

		void replace(size_t index, char c)
		{
			str[index] = c;
		}

		void replaceAll(char c, char r)
		{
			while (contains(c))
			{
				size_t index = indexOf(c);
				replace(index, r);
			}
		}

		String add(String string)
		{
			std::stringstream ss;
			ss << this << string;
			return ss.str().c_str();
		}

		String remove(String string)
		{
			String firstHalf = substring(0, indexOf(string));
			String secondHalf = substring(indexOf(string) + string.length(), length());
			return firstHalf + secondHalf;
		}

		String removeLast(String string)
		{
			String firstHalf = substring(0, lastIndexOf(string));
			String secondHalf = substring(lastIndexOf(string) + string.length(), length());
			return firstHalf + secondHalf;
		}

		String removeAll(String string)
		{
			String holder = copy();
			while (holder.contains(string.str))
			{
				holder = holder.remove(string);
			}
			return holder;
		}

		String flip()
		{
            int n = str.length();

            std::string str_ = str;

            // Swap character starting from two
            // corners
            for (int i = 0; i < n / 2; i++)
                std::swap(str_[i], str_[n - i - 1]);

            return str_;
		}

		void reset()
		{
			str = "";
		}

		static std::stringstream toStringStream(String str)
		{
			std::stringstream ss;
			ss << str;

			return ss;
		}

		template<typename T>
		T convertToType()
		{
			T value;
			String::toStringStream(copy()) >> value;
			return value;
		}

		String copy()
		{
			return String(str.c_str());
		}

		const std::string& string() const
		{
			return str;
		}

		const char* c_str() const
		{
            return str.c_str();
		}

		char operator [](size_t index)
		{
			if (index > length())
			{
				return charAt(length());
			}
			else if (index < 0)
			{
				return charAt(0);
			}

			return charAt(index);
		}

		template<typename T>
		friend String operator + (String string, T string2)
		{
			std::stringstream ss;
			ss << string << string2;
			return ss.str();
		}

		template<typename T>
		friend void operator += (String& string, T string2)
		{
			string = string + string2;
		}

		template<typename T>
		friend String operator - (String string, T string2)
		{
			std::stringstream ss;
			ss << string.remove(string2);
			return ss.str();
		}

		template<typename T>
		friend void operator -= (String& string, T string2)
		{
			string = string - string2;
		}

		friend bool operator == (String string, String string2)
		{
			if (string.str == string2.str)
				return true;
			return false;
		}

		friend bool operator != (String string, String string2)
		{
			if (string.str != string2.str)
				return true;
			return false;
		}

		friend std::ostream& operator << (std::ostream& os, String string)
		{
			os << string.str;
			return os;
		}

		friend std::istream& operator >> (std::istream& is, String& string)
		{
			is >> string.str;
			return is;
		}
	};
}
