#include "header.h"
#include <filesystem>




int main() {

	cout << filesystem::current_path() << endl; // 어느 위치에서 실행되는지 출력

	vector<int> lastindexmap;
	vector <Todo> td;
	string input;
	int input1;
	string input2;
	int num = 1;

	loadFromfile(td, num);

	map <string, function<void(string)>> commends;
	// add
	commends["add"] = [&](string arg) {
		if (arg.empty() || isblank(arg)) {
			cout << "내용을 입력하세요" << endl;
			return;
		}
		addTodos(td, num, arg);
		saveTofile(td);
		};
	//remove
	commends["remove"] = [&](string arg) {
		if (arg.empty() || isblank(arg)) {
			cout << "내용을 입력하세요" << endl;
			return;
		}
		removeTodo3(td, arg);
		if (td.empty()) num = 1;
		saveTofile(td);
		};
	//doing
	commends["doing"] = [&](string arg) {
		if (!safe_stoi(arg, input1)) {
			cout << "wrong number" << endl;
			return;
		}
		if (updateStatus(td, input1, status::DOING)) {
			cout << "\n변경이 완료되었습니다" << endl;
			saveTofile(td);
		}
	};
	//done
	commends["done"] = [&](string arg) {
		if (!safe_stoi(arg, input1)) {
			cout << "wrong number" << endl;
			return;
		}
		if (updateStatus(td, input1, status::DONE)) {
			cout << "\n변경이 완료되었습니다" << endl;
			saveTofile(td);
		}
	};
	//todo
	commends["todo"] = [&](string arg) {
		if (!safe_stoi(arg, input1)) {
			cout << "wrong number" << endl;
			return;
		}
		if (updateStatus(td, input1, status::TODO)) {
			cout << "\n변경이 완료되었습니다" << endl;
			saveTofile(td);
		}
	};

	//list
	commends["list"] = [&](string) {
		printTodos(td);
	};

	while (true) {
	
		
		
		cout << ">";
		getline(cin, input);

		string commend;
		string argument;
		size_t pos = input.find(' '); // input에서 공백을 찾고 그 위치를 pos에 저장

		if (pos == string::npos) { // 공백을 찾지 못하면
			commend = input; // input을 커맨드에 넣고 list exit 등
			argument = ""; // argument에는 아무것도 안넣음
		}
		else {
			commend = input.substr(0, pos); // 0 부터 공백전까지의 문자
			argument = input.substr(pos + 1); // 공백 다음 문자열 제한 없음 있는거 다읽음
		}

		
		if (commend == "exit") {
			saveTofile(td);
			cout << "program exit" << "\n\n";
			break;
		}
		// map
		auto it = commends.find(commend);

		if (it != commends.end()) {
			it->second(argument);
		}
		else {
			cout << "알 수 없는 명령어" << endl;
		}

	}




	return 0;
}