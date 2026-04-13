#include "header.h"
#include <filesystem>




int main() {

	cout << filesystem::current_path() << endl; // 어느 위치에서 실행되는지 출력

	vector <Todo> td;
	vector<int> displayToid;
	string input;
	int input1;
	int num = 1;

	loadFromfile(td, num);

	map <string, function<void(string)>> commends;
	// add
	commends["add"] = [&](string arg) {
		if (arg.empty() || isblank(arg) || arg.find('|') != string::npos) {
			cout << "잘못된 입력입니다.('|' 문자는 사용하실 수 없습니다)" << endl;
			return;
		}
		addTodos(td, num, arg);
		saveTofile(td);
		printTodos(td, displayToid);
		};
	//remove
	commends["remove"] = [&](string arg) {
		if (arg.empty() || isblank(arg)) {
			cout << "내용을 입력하세요" << endl;
			return;
		}
		string upperarg = arg;
		for (char& c : upperarg) {
			c = toupper(c);
		}

		if (upperarg == "TODO" || upperarg == "DOING" || upperarg == "DONE") {
			if (removeTodo3(td, arg)) {
				saveTofile(td);
				printTodos(td, displayToid);
				cout << "삭제가 완료되었습니다." << endl;
			}
			else {
				cout << "삭제할 항목이 없습니다." << endl;
			}
			return;
		}

		int displayindex;
		if (!safe_stoi(arg, displayindex)) {
			cout << "숫자를 입력하세요" << endl;
			return;
		}

		int realid;
		if (!getrealIdfromdisplayindex(displayToid, displayindex, realid)) {
			cout << "잘못된 번호입니다." << endl;
			return;
		}

		if (removeTodo3(td, to_string(realid))) {
			saveTofile(td);
			printTodos(td, displayToid);
			cout << "삭제가 완료되었습니다." << endl;
		}
		else {
			cout << "삭제할 항목이 없습니다." << endl;
		}
		};
	//doing
	commends["doing"] = [&](string arg) {
		if (arg.empty() || isblank(arg)) {
			cout << "내용을 입력하세요" << endl;
			return;
		}

		int displayIndex;
		if (!safe_stoi(arg, displayIndex)) {
			cout << "숫자를 입력하세요" << endl;
			return;
		}

		int realId;
		if (!getrealIdfromdisplayindex(displayToid, displayIndex, realId)) {
			cout << "잘못된 번호입니다." << endl;
			return;
		}

		if (updateStatus(td, realId, status::DOING)) {
			saveTofile(td);
			printTodos(td, displayToid);
		}
		else {
			cout << "상태 변경 실패" << endl;
		}
	};
	//done
	commends["done"] = [&](string arg) {
		if (arg.empty() || isblank(arg)) {
			cout << "내용을 입력하세요" << endl;
			return;
		}

		int displayIndex;
		if (!safe_stoi(arg, displayIndex)) {
			cout << "숫자를 입력하세요" << endl;
			return;
		}

		int realId;
		if (!getrealIdfromdisplayindex(displayToid, displayIndex, realId)) {
			cout << "잘못된 번호입니다." << endl;
			return;
		}

		if (updateStatus(td, realId, status::DONE)) {
			saveTofile(td);
			printTodos(td, displayToid);
		}
		else {
			cout << "상태 변경 실패" << endl;
		}
	};
	//todo
	commends["todo"] = [&](string arg) {
		if (arg.empty() || isblank(arg)) {
			cout << "내용을 입력하세요" << endl;
			return;
		}

		int displayIndex;
		if (!safe_stoi(arg, displayIndex)) {
			cout << "숫자를 입력하세요" << endl;
			return;
		}

		int realId;
		if (!getrealIdfromdisplayindex(displayToid, displayIndex, realId)) {
			cout << "잘못된 번호입니다." << endl;
			return;
		}

		if (updateStatus(td, realId, status::TODO)) {
			saveTofile(td);
			printTodos(td, displayToid);
		}
		else {
			cout << "상태 변경 실패" << endl;
		}
	};

	//list
	commends["list"] = [&](string) {
		printTodos(td, displayToid);
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