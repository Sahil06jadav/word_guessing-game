#include "iostream"
#include "string"
#include "cstdlib"
#include "ctime"
#include "limits"

using namespace std;

enum game_state{												//enum to decide current gamestate
	Menu=1,Running,Ended
};

enum spec_attack{										//enum to decide either 2Xattack or <->attack
	double_damage,revert_attack
};

enum spec_choices{
	one=1,two,three,four,five,six,seven,eight,nine,ten
};

template <typename T>											//template to validate inputs
T input_validate(){
	T value;
	while(true){
		if(cin >> value){
			return value;
		}
		cout<<"Incorrect input type, Try Again:-";
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
	}
}

template <typename T>										//template to take input with displaying something
T take_input(const string& display){
	T input;
	cout<<display;
	input=input_validate<T>();
	return input;
}

class Game;
class Player;

class Game{														// the class that works as a middle man
private:
	game_state state;
	int turn_deciding_guess;

private:
	void game_state_setter(game_state state);					//sets the state of the game
public:
	Game()
			:state(Menu){
				
			}
			
	void game_run(Player& player_1,Player& player_2);			//state to run and calls initiation and turn decider
	void game_initiation(Player& player_1,Player& player_2);	//just prints initial healths of players
	void turn_decider(Player& player_1,Player& player_2);		//decides first turn from both player
	void game_loop(Player& player_1,Player& player_2);			//checks health and attacks and turn_no ++ and spec(1/0)
	void game_guess(Player& attacker,Player& defender);		//checks if defender guess and set def_succ
	void game_attack(Player& attacker,Player& defender);		//perform att and spec_att and decr health accordingly
 	void game_spec_attack(Player& attacker,Player& defender);	//asks which special attack and performs it
	void game_end(Player& winner);								//set state to end and print winner
	
};

class Player{
private:
	bool winner;
	int health;
	bool special_ready;
	bool turn;
	int turn_number;
	bool defense_success;
	int guess;
	spec_attack special_choice;
	spec_choices special_guess;

public:
	string name;


public:

	Player(string name)
			:winner(0),health(100),special_ready(0),turn(0),turn_number(0),defense_success(0)
			,guess(0),special_choice(double_damage),special_guess(one),name(name){
			}
	
	bool get_turn() const{
		return this->turn;
	}
	void set_turn(bool turn){
		this->turn = turn;
	}
	int get_guess() const{
		return this->guess;
	}
	void set_guess(int guess){
		this->guess = guess;
	}
	void increment_turn_number(){
		this->turn_number++;
	}
	int get_turn_number() const{
		return this->turn_number;
	}
	bool get_defense_success()const{
		return this->defense_success;
	}
	void set_defense_success(bool x){
		this->defense_success = x;
	}
	int get_health() const{
		return this->health;
	}
	bool get_special_ready() const{
		return this->special_ready;
	}
	void set_special_ready(bool x){
		this->special_ready=x;
	}
	void decrement_double(Player& opponent){
		this->health -= 40;
		if(this-> health <= 0){
			opponent.winner = 1;
		}
	}
	void decrement_more(Player& opponent){
		this->health -= 20;
		if(this-> health <= 0){
			opponent.winner = 1;
		}
	}
	void decrement_less(Player& opponent){
		this->health -= 12;
		if(this-> health <= 0){
			opponent.winner = 1;
		}
	}
	void set_special(spec_attack x){
		this->special_choice = x;
	}
	spec_attack get_special() const{
		return this->special_choice;
	}
	bool get_winner(){
		return this->winner;
	}
};

void Game::game_state_setter(game_state state){
	this->state = state;
}

void Game::game_run(Player& player_1,Player& player_2){
	game_state_setter(Running);
	game_initiation(player_1,player_2);
	turn_decider(player_1,player_2);
	while(this->state == Running){
		game_loop(player_1,player_2);
	}
}

void Game::game_end(Player& winner){
	game_state_setter(Ended);
	cout<<winner.name<<" HAS WON THE GAME"<<endl;
	return;
}

void Game::game_initiation(Player& player_1,Player& player_2){
	cout<<"INITIALIZING THE PLAYERS PLEASE WAIT TILL IT STARTS......."<<endl;
	cout<< (player_1.name) << "'s health = " << (player_1.get_health()) <<endl;
	cout<< (player_2.name) << "'s health = " << (player_2.get_health()) <<endl;
}

void Game::turn_decider(Player& player_1,Player& player_2){
	int p1_diff,p2_diff;
	int p1_guess,p2_guess;
	p1_guess=0;p2_guess=0;
	this->turn_deciding_guess = rand()%10 + 1;
	while(!((p1_guess <= 10 && p2_guess <= 10) && (p1_guess >= 1 && p2_guess >= 1))){
	cout << player_1.name << " enter a number from 1 to 10: ";
	p1_guess = take_input<int>("");
	player_1.set_guess(p1_guess);
	cout << player_2.name << " enter a number from 1 to 10: ";
	p2_guess = take_input<int>("");
	player_2.set_guess(p2_guess);
	}
	cout<<"The number to guess was: "<<this->turn_deciding_guess<<endl;
	p1_diff = (this->turn_deciding_guess) - (player_1.get_guess());
	p2_diff = (this->turn_deciding_guess) - (player_2.get_guess()); 
	p1_diff = p1_diff<0 ? -p1_diff : p1_diff;
	p2_diff = p2_diff<0 ? -p2_diff : p2_diff;
	
	if(p1_diff <= p2_diff){
		player_1.set_turn(1);
		cout << player_1.name << " will start the game "<<endl;
	}
	else{
		player_2.set_turn(1);
		cout << player_2.name << " will start the game "<<endl;
	}
}

void Game::game_loop(Player& player_1,Player& player_2){
	if(player_1.get_winner()){
		game_end(player_1);
		return;
	}
	if(player_2.get_winner()){
		game_end(player_2);
		return;
	}
	if(player_1.get_turn()){
		game_attack(player_1,player_2);
		player_1.increment_turn_number();
		if(((player_1.get_turn_number()) % 4 ==0) && ((player_1.get_turn_number()) != 0)) {
			player_1.set_special_ready(1);
		}
	}
	else{
		game_attack(player_2,player_1);
		player_2.increment_turn_number();
		if(((player_2.get_turn_number()) % 4 ==0) && ((player_2.get_turn_number()) != 0)) {
			player_2.set_special_ready(1);
		}
	}
}

void Game::game_guess(Player& attacker,Player& defender){
	int length_decided;
	string def_guess;
	cout << attacker.name << " enter the length of the word you want "<< defender.name <<" to guess:";
	length_decided = take_input<int>("");
	cout<<defender.name;
	def_guess = take_input<string>(string(" please enter a word of length ") + std::to_string(length_decided) + string(":"));
	if(length_decided == int(def_guess.length())){
		defender.set_defense_success(1);
		cout << defender.name << " has defended the attack to some extent!" << endl;
	}
	else{
		defender.set_defense_success(0);
		cout << defender.name << " was unable to defend the attack!" << endl;
	}
}

void Game::game_attack(Player& attacker,Player& defender){
	if(attacker.get_special_ready()){
		bool perform_special;
		cout << attacker.name <<" do you want to use your special?(Yes=1 / N0=0): ";
		perform_special = take_input<bool>("");
		if (perform_special){
			game_spec_attack(attacker,defender);
			cout << defender.name << "'s health now is: "<< defender.get_health()<<endl; 
			attacker.set_special_ready(0);
			return;
		}
	}
	if(attacker.get_turn()){
		game_guess(attacker,defender);
		if(defender.get_defense_success()){
			defender.decrement_less(attacker);
			cout << defender.name << "'s health now is: "<< defender.get_health()<<endl; 
		}
		else{
			defender.decrement_more(attacker);
			cout << defender.name << "'s health now is: "<< defender.get_health()<<endl;
		}
		attacker.set_turn(0);
		defender.set_turn(1);
	}
}

void Game::game_spec_attack(Player& attacker,Player& defender){
	int choice;
	cout<<attacker.name<<" what is the special attack u want to perform?(Double-Damage=1/Attack-Revert=0):";
	choice = take_input<bool>("");
	
	if(choice){
		attacker.set_special(double_damage);
	}
	else{
		attacker.set_special(revert_attack);
	}
	
	if(attacker.get_special() == double_damage){
		int attacker_guess=-1;
		int defender_guess=-1;
		while(!((attacker_guess >= 0 && attacker_guess <= 5) && (defender_guess >= 0 && defender_guess <= 5))) {
		cout << attacker.name <<" please enter a number from 0 to 5 :";
		attacker_guess=take_input<int>("");
		cout << defender.name <<" please enter a number from 0 to 5 :";
		defender_guess=take_input<int>("");
		}
		if(attacker_guess==defender_guess){
			defender.decrement_double(attacker);
			cout<<"Special Attack hit successfully!!!"<<endl;
		}
		else{
			cout<<"Special Attack didnt hit!!!"<<endl;
		}
		attacker.set_turn(0);
		defender.set_turn(1);
	}
	else{
		cout<<"Turns Reversed Successfully!!!"<<endl;
		attacker.set_turn(1);
		defender.set_turn(0);
	}
}

int main(){
	srand(time(0));
	Game game;
	string player1,player2;
	char temp='a';
	while(temp != 'Y' && temp != 'N' && temp != 'y' && temp != 'n'){
		cout<<"DO YOU WANT TO START THE GAME?"<<endl;
		temp=take_input<char>("Y/N:-");
	}
	if(temp == 'n' || temp == 'N'){
		cout<<"MOFO if you did not want to start the game then why did you run this!!!"<<endl;
		return 1;
	}
	else{
		cout << "Enter player-1 name: ";
		player1 = take_input<string>("");
		cout << "Enter player-2 name: ";
		player2 = take_input<string>("");
		Player player_1 = Player(player1);
		Player player_2 = Player(player2);
		game.game_run(player_1,player_2);
	}
}