#ifndef __SZCOLORED_H__
#define __SZCOLORED_H__

#include <iostream>

namespace sz
{
	template <typename ItemT, typename CharT> class colored
	{
	public:
		colored(const CharT* colorStr, const ItemT& item)
			: m_colorStr(colorStr), m_item(item)
		{
		}

		// Disable Copy
		colored(const colored& other) = delete;
		colored& operator=(const colored& other) = delete;

		// Enable Move
		colored(colored&& other)
			: m_colorStr(other.m_colorStr), m_item(other.m_item)
		{
		}

		// The parameter is an R-Value reference since we don't want retained copies of this class.
		friend std::basic_ostream<CharT>& operator<<(std::basic_ostream<CharT>& os, colored&& item)
		{
			static const CharT strPrefix[3]{ '\x1b', '[', '\0' };
			static const CharT strSuffix[5]{ '\x1b', '[', '0', 'm', '\0' };

			os << strPrefix << item.m_colorStr << CharT('m') << item.m_item << strSuffix;

			return os;
		}

	private:
		const CharT* m_colorStr;
		const ItemT& m_item;
	};

	template <typename ItemT, typename CharT> colored<ItemT, CharT> with_color(const CharT* colorStr, const ItemT& item)
	{
		return colored<ItemT, CharT>(colorStr, item);
	}
}

#endif // __SZCOLORED_H__
