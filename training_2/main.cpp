
/*      -std=c++14 -O3 -Wall
 *
 *		Дано число. В этом числе необходимо изменить одну цифру таким образом, чтобы
 *		новое число делилось на 3 и было бы максимально возможным. В исходном числе нужно
 *		обязательно изменить одну цифру, даже если исходное число уже делилось на 3.
 * 		Программа получает на вход одно длинное натуральное число. Длина числа может
 *		достигать 100 цифр.
 *
 *		Программа должна вывести другое натуральное число, удовлетворяющее условиям:
 *
 *		1. Новое число должно отличаться от данного ровно одной цифрой.
 *		2. Новое число должно делиться на 3.
 *		3. Новое число должно быть максимально возможным из всех таких
*/

// конечно, данное решение можно оптимизировать, но и без этого работает крайне быстро, даже на числах до 100 цифр.

#include <iostream>
using namespace std;

inline short char_code_offset()
{
	static short i;	char c = '0';
	if (!i) for (;c!=char(i);++i);
	return i;
}

string & calc(string & val)
{
	string::iterator _beg(val.begin());
	short offset = char_code_offset();
	short j, divided;
	char saved_symbol;
	while ( _beg != end(val) )
	{
		// первый проход на повышение от
		// старшего разряда к младшему desu
		j = *_beg-offset;
		divided = 0;
		for( short i = 9; i>j; i-- )
		{
			saved_symbol = *_beg;
			*_beg = i+offset;
			for(size_t k=0; k<val.length(); k++)
			{   // этот цикл можно вынести выше, чтобы
				// не гонять лишний раз вычисление dividet
				// но так проще код, и работает всеравно быстро
				divided += (val[k]-offset)%3;
			}
			if (!(divided%3)) return val;
			else divided = 0;
			
			*_beg = saved_symbol;
		}
		++_beg;
	}
	// вычисление с другого конца (худший вриант)
	string::reverse_iterator _rbeg(val.rbegin());
	while ( _rbeg != val.rend() )
	{
		j = *_rbeg-offset-1;
		divided = 0;
		for( ; j>=0; j-- )
		{
			saved_symbol = *_rbeg;
			*_rbeg = j+offset;
			for(size_t k=0; k<val.length(); k++)
			{
				divided += (val[k]-offset)%3;
			}
			if (!(divided%3)) return val;
			else divided = 0;
			*_rbeg = saved_symbol;
		}
		++_rbeg;
	}
	return val; // for compiler happy
}

int main (void)
{
	
	string number;
	cout << "Введите число: ";
	cin >> number;
	cout << calc(number) << endl;
    return 0;
}

