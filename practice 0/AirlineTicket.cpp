#include <string>
#include <iostream>

using namespace std;


class AirlineTicket
{
public:
	AirlineTicket();
	~AirlineTicket();
	

	double calculatePriceInDollars() const;
	
	std::string getPassengerName() const;
	void setPassengerName(std::string &name);

	int getNumberOfMiles() const;
	void setNumberOfMiles(int miles);
	
	bool hasEliteSuperRewardsStatus() const;
	void setHasEliteSuperRewardsStatus(bool status);

private:
 
	std::string m_passengerName;
    int m_numberOfMiles;
    bool m_hasEliteSuperRewardsStatus;
};



AirlineTicket::AirlineTicket()
	: m_passengerName("Unknown Passenger")
	, m_numberOfMiles(0)
	, m_hasEliteSuperRewardsStatus(false )
{
}

AirlineTicket::~AirlineTicket()
{
	// Nothing to do in terms of cleanup
}

double AirlineTicket::calculatePriceInDollars() const
{
	if (hasEliteSuperRewardsStatus()) {
		// Elite Super Rewards customers fly for free!
		return 0;
	}

	// The cost of the ticket is the number of miles times 0.1.
	// Real airlines probably have a more complicated formula!
	return getNumberOfMiles() * 0.1;
}

std::string AirlineTicket::getPassengerName() const
{
	return m_passengerName;
}

void AirlineTicket::setPassengerName(std::string &name)
{
	m_passengerName = name;
}

int AirlineTicket::getNumberOfMiles() const
{
	return m_numberOfMiles;
}

void AirlineTicket::setNumberOfMiles(int miles)
{
	m_numberOfMiles = miles;
}

bool AirlineTicket::hasEliteSuperRewardsStatus() const
{
	return m_hasEliteSuperRewardsStatus;
}

void AirlineTicket::setHasEliteSuperRewardsStatus(bool status)
{
	m_hasEliteSuperRewardsStatus = status;
}




int main()
{
	AirlineTicket myTicket;  // Stack-based AirlineTicket
	std::string name = "Sherman T. Socketwrench";
	myTicket.setPassengerName(name);
	myTicket.setNumberOfMiles(700);
	double cost = myTicket.calculatePriceInDollars() ;
	cout << "This ticket will cost $" << cost << endl;
} 