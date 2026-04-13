#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <algorithm>
#include <functional>
#include <map>
#include <cctype>

using namespace std;

enum class status {
	TODO, DOING, DONE
};

struct Todo {
	string task;
	status st;
	int num;
};

string statusTostring(status s) {
	if (s == status::TODO) return " [할 일]";
	if (s == status::DOING) return " [진행중]";
	if (s == status::DONE) return " [완료]";
	return "UNKNOWN";  
}

status stringTostatus(const string& s) {
	if (s == "TODO") return status::TODO;
	if (s == "DOING") return status::DOING;
	if (s == "DONE") return status::DONE;
	return status::TODO;
}

string getString() {
	string str;
	getline(cin, str);
	return str;
}

bool isblank(const string& s) {
	for (char c : s) {
		if (!isspace(c)) return false; // argument에 공백이 없으면 false를 밷어라
	}
	return true; // 공백이 있으면 true
}

void printTodos(const vector<Todo>& td, vector<int>& displayToid) {
	displayToid.clear();

	if (td.empty()) {
		return;
	}
	int displayindex = 1;

	for (size_t i = 0; i < td.size(); i++) {
		cout << i + 1 << ". " << td[i].task << statusTostring(td[i].st) << "\n\n";
		displayToid.push_back(td[i].num);
	}
}

void addTodos(vector<Todo>& td, int& nextnum, const string& task) {
	td.push_back({ task, status::TODO, nextnum++ });
	cout << "add complete" << "\n\n";
}

bool updateStatus(vector<Todo>& td, int num, status newstatus) {
	for (auto& t : td) {
		if (t.num == num) {
			t.st = newstatus;
			return true;
		}
	}
	cout << "number not found" << "\n";
	return false;
}

bool safe_stoi(const string& s, int& result) { // 그래서 예외처리하는 함수 참 거짓만 판단하기때문에 bool
	try {
		size_t idx;
		result = stoi(s, &idx); // s 에는 numstr 사용자 입력 문자열 idx에는 앞에서부터 숫자를 어디까지 읽었는지 기록

		if (idx != s.size()) return false; // 입력이 123 일때 idx 에는 3이 들어감 왜? idx는 마지막 인덱스 즉 s[2] 가 아니라 몇 글자를 읽었는지 length 임 그래서 3 size() 도 마지막인덱스 번호가아니라 요소의 개수를 의미함
		// 입력이 123abc일때 idx는 숫자의 마지막까지 읽으니까 3 size는 전체를 길이니까 6 다름 false
		return true;
	}
	catch (...) {
		return false;
	}
}

void loadFromfile(vector<Todo>& td, int& nextnum) {
	ifstream file("todos.txt");
	if (!file.is_open()) {
		return;
	}
	string line;

	while (getline(file, line)) { // 파일에있는 텍스트를 한줄씩 line 으로 getline 의 첫번째 인자는 입력소스 cin으로 키보드(외부)입력이 될 수 있고 지금 경우에는 파일에있는 텍스트인 것
		if (line.empty()) continue;
		stringstream ss(line); // line 에 들어간 텍스트를 키보드입력처럼 바꿔준다
		string numstr, task, statusStr;
		 
		if (!getline(ss, numstr, '|') ||
			!getline(ss, task, '|') ||
			!getline(ss, statusStr, '|'))
			continue;// ss 자리에 cin을 넣으면 내가 키보드로 입력해야겠지만 ss를 넣어서 텍스트가 바로 들어감 | 전까지의 텍스트를 numstr 에 저장 왜? getline 의 세번째 인자는 구분자 즉 구분자 전까지만 읽는다
		
		

		int num; // "1" 숫자도 문자열 형태로 들어가있으니까 string to int stoi 를 써서 숫자로 형변환 근데 이건 예외처리 반드시 필요 숫자가 아니라 이상한 입력이 들어오면 프로젝트 터짐
		if (!safe_stoi(numstr, num)) continue;
		td.push_back({ task, stringTostatus(statusStr), num }); // 벡터에 집어넣기

		if (num >= nextnum) nextnum = num + 1; // 기존 데이터에 num가 2까지 있다면 기존 nextnum = 1 보다 크니까 넥스트 넘버에 2 + 1 3을 넣는다 새로 넣는데이터가 3부터 시작
	}
}

void saveTofile(const vector<Todo>& td) {
	ofstream file("todos.txt");

	for (const auto& t : td) {
		file << t.num << "|"
			<< t.task << "|"
			<< (t.st == status::TODO ? "TODO" :
				t.st == status::DOING ? "DOING" : "DONE")
			<< "\n";
	}
}

bool removeTodo(vector <Todo>& td, int id) {
	for (auto it = td.begin(); it != td.end(); ++it) { // it는 begin 첫번째에서 시작 td.end() 마지막요소 다음번째가 아니면 루프 계속 돌아라 즉 마지막요소 다음까지 돌기 ++it 1씩 증가 it++ 써도 상관없음
		if (it->num == id) {
			td.erase(it);
			return true;
		}
	}
	return false;
}

bool removeTodo1(vector<Todo>& td, int id) {
	auto new_end = remove_if(td.begin(), td.end(), [id](const Todo& t) { return t.num == id; }); // removeif(범위 시작, 끝 end는 마지막 요소 다음까지, 람다함수 외부에서 들어온 값 id를 [id] 로 복사
																								//  const 읽기전용 수정불가 Todo& t 왜 벡터 안붙임? 벡터 전체를 가져오는게 아니라 1.운동 [할 일] 이렇게
																								// 요소 하나씩 가져와서 검사 return t.num == id 이건 id 즉 내가 입력한 값이 t.num 와 같으면 true 다르면 false
																								// remove_if는 ture를 삭제함 id와 t.num 가 같으면 삭제함
	if (new_end == td.end()) return false;
	td.erase(new_end, td.end());
	return true;
}

bool removeTodo2(vector<Todo>& td, const string& s) { // 최종버전 근데 너무 길다 그냥 복붙여러개 한 수준 
	
	if (s == "done") {
		auto new_end = remove_if(td.begin(), td.end(), [](const Todo& t) {return t.st == status::DONE; });

		if (new_end == td.end()) return false;
		td.erase(new_end, td.end());
		return true;
	}
	else if (s == "doing") {
		auto new_end = remove_if(td.begin(), td.end(), [](const Todo& t) {return t.st == status::DOING; });

		if (new_end == td.end()) return false;
		td.erase(new_end, td.end());
		return true;
	}
	else if (s == "todo") {
		auto new_end = remove_if(td.begin(), td.end(), [](const Todo& t) {return t.st == status::TODO; });

		if (new_end == td.end()) return false;
		td.erase(new_end, td.end());
		return true;
	}
	else {
		int id;
		if (!safe_stoi(s, id)) return false;
		auto new_end = remove_if(td.begin(), td.end(), [id](const Todo& t) {return t.num == id; });

		if (new_end == td.end()) return false;
		td.erase(new_end, td.end());
		return true;
	}
}

bool removeIf(vector <Todo>& td, function<bool(const Todo&)> cond) {
	auto new_end = remove_if(td.begin(), td.end(), cond);
	if (new_end == td.end()) return false;
	td.erase(new_end, td.end());
	return true;
}

bool removeTodo3(vector <Todo>& td, const string& s) {
	string upper = s;
	for (char& c : upper) {
		c = toupper(c);
	}

	if (upper == "TODO" || upper == "DOING" || upper == "DONE") {
		

		status st = stringTostatus(upper);

		return removeIf(td, [st](const Todo& t) {return t.st == st; });
	}

	int id;
	if (!safe_stoi(s, id)) return false;

	return removeIf(td, [id](const Todo& t) {return t.num == id; });
}

bool getrealIdfromdisplayindex(const vector<int>& displayToid, int displayindex, int& realid) {
	if (displayindex < 1 || displayindex >(int)displayToid.size()) {
		return false;
	}

	realid = displayToid[displayindex - 1];
	return true;
}