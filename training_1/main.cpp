#include <iostream>

/*
 * У красильщика имеется N предметов, которые он должен покрасить. Красильщик красит i-й предмет за время pi, после чего предмет сохнет время qi.
 * Одновременно красильщик может красить только один предмет, а сохнуть одновременно может произвольное число предметов. Определите последовательность,
 * в которой красильщик должен красить предметы, чтобы сделать суммарное время выполнения работы минимальным 
 * (время считается от начала работы до того момента, когда высохнут все предметы). 
 *
 * Вход:    Выход:        Вход:    Выход: 
 * 3                      4
 * 10 30    1 3 2         80 1     2 4 3 1
 * 10 10                  40 30
 * 20 25                  10 10
 *                        20 35
 */

struct Data
{
	unsigned colorTime;
	unsigned dryTime;
	bool     finish;
	Data (unsigned, unsigned);
};

Data::Data(unsigned a, unsigned b):colorTime(a), dryTime(b), finish(0) { }

struct DataContainer
{
	struct simplePair
	{
		unsigned first; bool second;
		explicit operator bool()
		{ return second; }
	};

	DataContainer(unsigned);
	~DataContainer();
	
	void choise(unsigned);
	void find_objects();
private:
	enum _LTypes { _color, _dry };
	Data * dataPtr;
	unsigned _size;
	unsigned _ready;
	simplePair find_max(DataContainer::_LTypes type, unsigned limit = 0);
	bool find_more_priority(unsigned color, unsigned dry);
};

DataContainer::DataContainer (unsigned size): dataPtr( (Data*)::operator new(sizeof(Data)*size, std::nothrow) ), _size(size), _ready(0)
{
    std::cout << "Input values COLOR_TIME space DRY_TIME:\n";
    for (size_t i = 0; i<_size; i++)
    {
        unsigned a, b;
        std::cout << "pair [" << i+1 << "] = ";
        std::cin  >> a >> b;
        new (dataPtr+i) Data(a, b);
    }
}

DataContainer::~DataContainer( )
{
	for ( unsigned i = 0; i < _size; i++ )
		dataPtr->~Data();
	::operator delete(dataPtr); 
}

void DataContainer::choise (unsigned i)
{
    dataPtr[i].finish = 1;
}

DataContainer::simplePair DataContainer::find_max (DataContainer::_LTypes type, unsigned limit)
{
	unsigned i, saved_i, max;
	unsigned (Data::*ptr);
	switch ( type )
	{
		case _color : ptr = &Data::colorTime;    break;
		case _dry :   ptr = &Data::dryTime;      break;
	}
	for ( max = i = 0; i < _size; ++i )
	{
		if (dataPtr[i].finish) { continue; }
		if ( max < dataPtr[i].*ptr )
		{
			if ( limit )
			{
				if ( dataPtr[i].*ptr < limit)
				{
					max = dataPtr[i].*ptr;
					saved_i = i;
				}
			}
			else
			{
				max = dataPtr[i].*ptr;
				saved_i = i;
			}
		}
	}
	return simplePair { saved_i, ( (max) ? true : false) };
}

bool DataContainer::find_more_priority(unsigned color, unsigned dry)
{
	unsigned j, i, max, last_max_i=unsigned(), good;
	unsigned time = dry;
	for ( j=0; j<_size-_ready; j++ )
	{
		good = 0; // маркер того что нашли объект, который стоит рассмотреть
		
		for ( max=i=0; i<_size; i++ )
		{
			// покрашеные элементы исключаются из рассмотрения
			if (dataPtr[i].finish)
			{
				continue;
			}
			
			// не смотрим элементы, время покраски которых больше чем время покраски
			// текущего кандидата на более приоритетный выбор
			if ( dataPtr[i].colorTime >= color )
			{
				continue;
			}

			// с помощью last_max_i исключаем последний найденный максимум
			// и тем самым делаем выборку максимумов, начиная с самого большого
			if ( max < dataPtr[i].colorTime && i != last_max_i )
			{
				max  = dataPtr[i].colorTime;
				good = 1;
				last_max_i = i;
			}
		}
		if (good)
		{
			// необходимо сравнить времена сушки кандидата на более приоритетную выборку
			// если это время становится  меньше, чем сумма времен покрасок остальных (невыбранных)
			// деталей ->  означает что если этот кандидат на приоритетную выборку будет выбран, то
			// случится простой процесса высыхания, поэтому данный кандидат не может быть выбран.
			if (time < max)
				return false;
			
			// от времени сушки отнимаем время покраски невыбранного элемента, тем самым,
			// как-бы, размещая во времени сушку этого кандидата, и покраску других элементов
			time -= max;
			if (time < dataPtr[last_max_i].dryTime)
			{
				// общее время увеличивается только в случае, если сушка одной из
				// невыбранных деталей превышает время сушки остальных, иначе
				// оно как бы просто размещается в уже существующем интервале
				// не увеличивая его
				time = dataPtr[last_max_i].dryTime;
			}
		}
	}
	if (dry < time)
		return true;
	return false;
}

void DataContainer::find_objects()
{
	unsigned pervois_maxColor = 0;
	simplePair maxColorIdx {0, false};
	simplePair maxDryIdx;
	while ( _ready != _size )
	{

		// сначала находим деталь с максимальным временем сушки
		maxDryIdx = find_max(_dry);
		
		// далее, необходимо проверить, нет ли деталей, с временем покраски, превышающим
		// время сушки, кандидата на выборку. Причем, если их несколько то
		// cначала выберется максимальный. В следующих итерациях поиск не будет брать
		// ранее найденный (уже обработанный) максимум, сужая интервал поиска.

		if (maxColorIdx)
		{
			// сужение интервала
			maxColorIdx = find_max(_color, dataPtr[pervois_maxColor].colorTime);
		}
		else 
		{
			// первичный поиск
			maxColorIdx = find_max(_color);
		
		}
		pervois_maxColor = maxColorIdx.first;

		// если найден элемент время покраски которого больше времени сушки кандидата на выборку
		if (maxColorIdx)
		{
			// проверяем, можно ли выбрать этот элемент, и если да, то он выбирается ранее чем кандидат
			if ( find_more_priority(dataPtr[maxColorIdx.first].colorTime, dataPtr[maxColorIdx.first].dryTime) )
			{
				choise(maxColorIdx.first);
				++_ready;
				std::cout << "ANS: "<< 1+maxColorIdx.first << std::endl;
			}
		}
		else
		{
			// если более приоритетных элементов нет, выбирается кандидат.
			choise(maxDryIdx.first);
			++_ready;
			std::cout << "ANS: "<< 1+maxDryIdx.first << std::endl;
		}
	}
}

int main()
{
    unsigned cnt;
    std::cout << "input number of objects: "; std::cin >> cnt;
    DataContainer(cnt).find_objects();
    return 0;
}
