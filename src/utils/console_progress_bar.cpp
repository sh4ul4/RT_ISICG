#include "console_progress_bar.hpp"

#include <iostream>
#include <algorithm>
#include <string>

namespace RT_ISICG
{
	void ConsoleProgressBar::start( const int p_nbTasks, const int p_barWidth )
	{
		_barWidth = std::max<int>( 2, p_barWidth - 2 );
		_nbTasks  = std::max<int>( 1, p_nbTasks - 1 );

		std::cout << "Warning ! Console progress bar impacts performance !" << std::endl;
		_nbTasksDone = 0;
		std::cout << "\r[";
		for ( int i = 0; i < _barWidth; ++i )
		{
			std::cout << " ";
		}
		std::cout << "] 0.00 %";
	}

	void ConsoleProgressBar::next( const int n )
	{
		_mutex.lock();
		_nbTasksDone += n;
		int			nbToDisplay = _nbTasksDone * _barWidth / _nbTasks;
		std::string strPercent	= std::to_string( _nbTasksDone * 100.f / _nbTasks );
		std::string percent( strPercent.begin(), strPercent.begin() + 4 );
		std::cout << "\r[";
		for ( int i = 0; i < _barWidth; ++i )
		{
			std::cout << ( i < nbToDisplay ? "-" : " " );
		}
		std::cout << "] " << percent << " % ";
		std::cout.flush();
		_mutex.unlock();
	}

	void ConsoleProgressBar::stop()
	{
		_nbTasksDone = 0;
		std::cout << std::endl;
	}
} // namespace VTX
