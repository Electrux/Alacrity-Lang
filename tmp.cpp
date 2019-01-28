#include <iostream>
#include <cmath>

int main()
{
	int num = 2;
	int sum = 0;
	int count = 0;
	while( count != 1000 ) {
		int sqr = std::sqrt( num );
		bool is_prime = true;
		for( int i = 2; i <= sqr; ++i ) {
			if( num % i == 0 ) { is_prime = false; break; }
		}
		if( is_prime ) {
			++count;
			sum += num;
		}
		++num;
	}
	std::cout << "Sum of first 1000 prime numbers is: " << sum << "\n";
	return 0;
}
