#include<iostream>
#include <string>     // std::string, std::stoi
#include <time.h>
#include<vector>
#include<fstream>
#include<sstream>
#include<algorithm>
#include<cctype>

// Fatima Rahimova 150180905


//since we should make a case-insensitive comparison, and since there is not any function which make all letters of string lowercase,
// i wrote this function by myself.
// this function takes a string as an input,
// and by using for loop, all letters of this input converts to the lowercase letters by using tolower() function.
//function returns the output which is input string after converting all letters to lowercase letters.
std::string incase(const std::string& input) {
	std::string str(input);
	for (int i = 0; i < input.length(); i++) {
		str[i] = std::tolower(input[i]);
	}
	return str;
}

struct Tweet
{
	//Tweet struct have all attributes of each tweet in the dataset
	int tweet_id;
	std::string tweet_datetime;
	int tweet_unixtime;
	std::string artist_name;
	std::string track_title;
	std::string country;
	//constructor for Tweet struct
	//since tweet_id and tweet_unixtime are strings,
	//by using stoi() function, i convert strings to integer
	Tweet(const std::vector<std::string>& fields) {
		tweet_id = stoi(fields[0]);
		tweet_datetime = fields[1];
		tweet_unixtime = stoi(fields[2]);
		artist_name = fields[3];
		track_title = fields[4];
		country = fields[5];
	}

	// for comparison operations i overloaded < operator
	// for case insensitive comparisons, i made all letters of each strings lowercase by using incase function
	bool operator<(const Tweet& other) const
	{
		std::string c1 = incase(this->country);
		std::string c2 = incase(other.country);
		std::string a1 = incase(this->artist_name);
		std::string a2 = incase(other.artist_name);
		if (c1 < c2) //if country name of current tweet is alphabetically lesser than other's country name
			return true;
		else if (c1 == c2) { // if country name names are same
			if (a1 < a2) // compare artist names
				return true;
			else if (a1 == a2 && this->tweet_unixtime < other.tweet_unixtime) // if country names and artist names are same, then check unix time
				return true;
		}
		return false;
	}
	// after overloaded < operator, I overloaded > operator too, for comparisons in sorting algorithm
	bool operator>(const Tweet& other) const
	{
		std::string c1 = incase(this->country);
		std::string c2 = incase(other.country);
		std::string a1 = incase(this->artist_name);
		std::string a2 = incase(other.artist_name);
		if (c1 > c2) //if country name of current tweet is alphabetically greater than other's country name
			return true;
		else if (c1 == c2) { // if country name names are same
			if (a1 > a2) // compare artist names
				return true;
			else if (a1 == a2 && this->tweet_unixtime > other.tweet_unixtime) // if country names and artist names are same, then check unix time
				return true;
		}
		return false;
	}
};
// read_data function reads csv file
// it has two inputs: first input is csv file which is in string format, and the second input is number of tweets to be sorted
std::vector<Tweet> read_data(const std::string& input_file, int N) {
	std::ifstream file(input_file);
	std::vector<Tweet> tweets; // i create vector which stores tweets
	
	std::string line; // variable to hold to current line in csv file
	std::getline(file, line); //consume to first line, since it is the header  
	while (std::getline(file, line)) { //read the line and if it is not empty, continue the loop
		std::istringstream s(line); //put line into a stream
		std::vector<std::string> fields; //deconstructed fields of the line

		std::string field; //variable to hold the current field
		while (std::getline(s, field, '\t')) { //read until the next tab
			fields.push_back(field); //push the read value
		}
		tweets.emplace_back(fields); //push a new Tweet by creating it from fields variable
		
		//if the N tweets are alread read, break
		if (tweets.size() >= N) 
			break;
	}

	file.close(); // close the file

	return tweets; // return tweets array
}
//writing data to the output csv file
void write_data(const std::vector<Tweet>& data, const std::string& output) {
	std::ofstream file(output); //create stream output file
	file << "tweet_id\ttweet_datetime\ttweet_unixtime\tartist_name\ttrack_title\tcountry\n"; // write the header row

	//for each tweet in the data array write its fields with tab separated 
	for (const Tweet& tweet : data) {
		file << tweet.tweet_id << '\t'
			<< tweet.tweet_datetime << '\t'
			<< tweet.tweet_unixtime << '\t'
			<< tweet.artist_name << '\t'
			<< tweet.track_title << '\t'
			<< tweet.country << '\n';
	}
	file.close(); // close the field
}

//I used std::sort() function for checking correctness of other functions before implementing my own sorting functions :)
//void sort_tweets(std::vector<Tweet>& data)
//{
//	std::sort(data.begin(), data.end(), [](const Tweet& a, const Tweet& b) {
//		return a < b;
//		});
//}

//Function takes the array of numbers as first input parameter which is vector of type Tweet
//the second input parameter, int begin is the beginning index of array
//the third parameter, int end is the last index of array

int partition(std::vector<Tweet>& input_array, int begin, int end) {

	//the last element of array is assigned as pivot
	Tweet pivot = input_array[end];
	int i = begin - 1; //index of smaller element
	for (int j = begin; j <= end - 1; j++) { // for all elements of the given input array
		if (input_array[j] < pivot) { // if the current element is smaller than pivot
			i++; //increment index of smaller element
			//swap the array[i] and array[j]
			Tweet temp = input_array[i];
			input_array[i] = input_array[j];
			input_array[j] = temp;
		}
	}
	//swap array[i+1] with pivot element
	Tweet temp = input_array[i + 1];
	input_array[i + 1] = input_array[end];
	input_array[end] = temp;
	return (i + 1);
}

//Function takes the array of numbers as first input parameter which is vector of type Tweet
//the second input parameter, int begin is the beginning index of array
//the third parameter, int end is the last index of array
void quicksort_deterministic(std::vector<Tweet>& data, int begin, int end) {
	if (begin < end) {
		int part = partition(data, begin, end); //find the index of partitioning
		quicksort_deterministic(data, begin, part - 1); // Sort elements before partition
		quicksort_deterministic(data, part + 1, end); // Sort elements after partition
	}
}

void sort_tweets_determ(std::vector<Tweet>& data) {
	clock_t runtime = clock(); //I used clock() function as suggested in hw booklet
	quicksort_deterministic(data, 0, data.size() - 1); // call deterministic quicksort algorithm with relevant function parameters
	runtime = clock() - runtime; 
	//print runtime of the deterministic quicksort algorithm
	std::cout << "Sorted in " << 1000*((float)runtime / CLOCKS_PER_SEC) << " milliseconds by using deterministic pivot selection" << std::endl;
}

//random function generates random number within the given (min, max) range
int random(int min, int max) {
	int t = min + rand() % (max - min);
	return t;
}



int part_random(std::vector<Tweet>& input_array, int begin, int end) {
	int rand = random(begin, end); // get random index

	//swap the random indexed number of array with the last number of the array
	Tweet temp = input_array[rand]; 
	input_array[rand] = input_array[end];
	input_array[end] = temp;
	int value = partition(input_array, begin, end);
	return value;
	
}


void quicksort_random(std::vector<Tweet>& array, int begin, int end) {
	if (begin < end) {
		int part = part_random(array, begin, end); // find the partitioning index
		quicksort_random(array, begin, part-1); //sort elements before partition
		quicksort_random(array, part + 1, end); //sort elements after partition
	}
}

void sort_tweets_random(std::vector<Tweet>& data) {
	clock_t runtime = clock(); //I used clock() function as suggested in hw booklet
	quicksort_random(data, 0, data.size() - 1); // call randomized quicksort algorithm with relevant function parameters
	runtime = clock() - runtime;
	//print runtime of the randomized quicksort algorithm
	std::cout << "Sorted in " << ((float)runtime / CLOCKS_PER_SEC)*1000 << " milliseconds by using randomized pivot selection" << std::endl;
}

int main(int argc, char* argv[]) {
	//take the arguments and assign them to proper variables
	int N = std::stoi(argv[1]); 
	std::string A = argv[2];
	std::string I = argv[3];
	std::string O = argv[4];
	srand(time(NULL)); // for generating random numbers
	
	std::vector<Tweet> data = read_data(I, N); // read the given numbers of data from csv file
	
	if (A == "deterministic") // if A input is deterministic call deterministic quicksort algorithm
		sort_tweets_determ(data);
	else // otherwise call randomized quicksort algorithm
		sort_tweets_random(data);
	
	write_data(data, O); // write data to the desired output csv file
	
	return 0;
}
