#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <algorithm>

// using namespace std;

std::ifstream input("f.txt");

int polynomial_number = 1;
std::string result = "0", remainder_str = "0", division;

bool go(std::string str, int paint = 1);

struct polynomial {
	long long a = 0, n = 0;
	polynomial *next = nullptr;
};

// Polynomial's number check
bool check_num(std::string a) {
	for (int i = 1; i < a.size(); ++i)
		if (!isdigit(a[i]) || (a[0] != '+' && a[0] != '-' && !isdigit(a[0]))) {
			std::cout << "Error number" << std::endl;
			return 0;
		}
	if (atoi(a.c_str()) <= 0 || atoi(a.c_str()) > polynomial_number - 1) {
		std::cout << "Error number" << std::endl;
		return 0;
	}
	return 1;
}

// Clearing the string from spaces and converting the pros and cons
void clear(std::string &str) { 
	if (str == "0")
		return;
	if (str[0] == '+')
		str.erase(0, 1);

	for (int i = 0; i < str.size(); ++i)
		if (str[i] == ' ') {
			str.erase(i, 1);
			--i;
		}
	for (int k = 0; k < str.size(); ++k) {
		for (int i = 0; i < str.size() - 1; ++i) {
			if (str[i] == '+' && (str[i + 1] == '+' || str[i + 1] == '-')) {
				str.erase(i, 1);
				--i;
			}
		}
		for (int i = 0; i < str.size() - 1; ++i) {
			if (str[i] == '-' && str[i + 1] == '-') {
				str.erase(i, 1);
				str[i] = '+';
				--i;
			}
		}
		for (int i = 0; i < str.size() - 1; ++i) {
			if (str[i] == '-' && str[i + 1] == '+') {
				str.erase(i, 1);
				str[i] = '-';
				--i;
			}
		}

		if (str[str.size() - 1] == '0' && str[str.size() - 2] == '^') {
			str.erase(str.size() - 1, 1);
			str.erase(str.size() - 1, 1);
			str.erase(str.size() - 1, 1);
		}

		if (str.empty())
			str += '1';
		if (str.back() == '-' || str.back() == '+')
			str += '1';
		if (str.back() == '*')
			str.erase(str.size() - 1, 1);
	}
}

struct base {
	int n = 0;
	polynomial *p = nullptr;
	base *next = nullptr;
	std::string polynomialial() {
		std::string str, a, n;
		std::stringstream s;
		polynomial *q = p;
		for (; q != nullptr && (q->a != 0 || q->n != 0); q = q->next) {
			str += "+";
			if (q->n == 1) {
				s << q->a;
				s >> a;
				if (abs(q->a) == 1 && q->n != 0)
					if (q->a == -1) {
						a = "-";
					} else {
						a.clear();
					}
				s.clear();
				str = str + a + "x";
			} else {
				s << q->a;
				s >> a;
				if (abs(q->a) == 1 && q->n != 0)
					if (q->a == -1)
						a = "-";
					else
					a.clear();
				s.clear();
				s << q->n;
				s >> n;
				s.clear();
				str = str + a + "x^" + n;
			}
		}

		if (str.empty())
			return "0";
		clear(str);
		return str;
	}
	void print_base(base *q) {
		system("cls");
		q = q->next;
		while (q != nullptr) {
			std::cout << q->n << ": " << q->polynomialial() << std::endl;
			q = q->next;
		}
	}
};

// Adding a monomial to a polynomial
void add_polynomial(polynomial *&L, std::string m) {
	long long A, N, i1, i2, i3;
	polynomial *p;
	A = atoll(m.c_str());
	i1 = (m[0] == '-' || m[0] == '+');
	for (i2 = 0; isdigit(m[i2 + i1]) && i2 + i1 < m.size(); ++i2); // Reading numbers 25x2+25x+x+25+x2-25x2-25y-y-25-y2
	for (i3 = 0; !isdigit(m[i3 + i2 + i1]) && i3 + i2 + i1 < m.size(); ++i3); // Reading the variable name if(N==) N++;
	if (i2 == 0) {
		N = atoll(m.c_str() + i1 + i3);
		A = 1;
		if (m[0] == '-')
			A = -1;
	} else {
		if (i3 == 0)
			N = 0;
		else if (i1 + i2 + i3 == m.size())
			N = 1;
		else
			N = atoll(m.c_str() + i1 + i2 + i3);
	}

	for (i2 = 0, i1 = 0; i2 < m.size(); ++i2)
		if (isdigit(m[i2]))
			i1++;
	if (!i1) {
		A = 1;
		N = 1;
	}

	if (m.size() == 2) {
		if ((m[0] == '-') && !isdigit(m[1])) {
			A = -1;
			N = 1;
		}
	}

	if (L->n < N || L->next == nullptr){
		if (N == 0) {
			L->a += A;
			return;
		}

		p = new polynomial;
		p->n = N;
		p->a = A;
		p->next = L;
		L = p;
		return;
	}

	for (p = L; p->next->n > N && p->next != nullptr; p = p->next);
	if (p->n == N) {
		p->a += A;
		return;
	}

	if (p->next->n == N) {
		p->next->a += A;
		return;
	}

	polynomial *q = new polynomial;
	q->a = A;
	q->n = N;
	q->next = p->next;
	p->next = q;
}

// Removing a monomial from a polynomial
void delete_monomial(polynomial *&L, polynomial *&p) {
	if (p == L) {
		L = p->next;
		delete p;
		p = L;
	} else {
		polynomial *q = p->next;
		*p = *q;
		delete q;
	}
}

// Traversing the entire polynomial and removing null monomials
void reload(polynomial *&L) {
	polynomial *p;
	for (p = L; p->next != nullptr && p->n != 0; p = p->next) {
		if (p->a == 0){
			delete_monomial(L, p);
			reload(L);
			break;
		}
	}
}

// Creating a new polynomial
void new_polynomial(polynomial *&p, std::string &str) {
	std::string pol;
	clear(str);
	pol += str[0];
	for (int i = 1; i < str.size(); add_polynomial(p, pol), pol.clear(), pol += str[i], ++i) {
		for (; i < str.size() && str[i] != '-' && str[i] != '+'; ++i)
			pol += str[i];
	}

	if (str.size() == 1)
		add_polynomial(p, str);
	reload(p);
}

base *B = new base;
base *b = B;
// Removing a polynomial from the list by number
void delete_polynomial(int n) {
	base *q = B, *k;
	for (; q->n != n; q = q->next);
	if (q == B) {
		B = B->next;
		for (k = B; k != nullptr; k = k->next)
			k->n--;
		--polynomial_number;
		delete q;
	} else {
		if (q->next == nullptr) {
			for (k = B; k->next != q; k = k->next);
			k->next = nullptr;
			b = k;
			delete q;
		} else{
			k = q->next;
			*q = *k;
			if (q->next == nullptr)
				b = q;
			delete k;
			for (; q != nullptr; q = q->next)
				q->n--;
		}
		--polynomial_number;
	}
}

// Asking the user about adding a polynomial to the database
void question(int k = 1 , std::string sniper = "", int j = 1) {
	std::string str;
	std::cout << sniper + "\nAdd to base? (Yes/No)";
	if (j && sniper == "0")
		k = 1;
	for (std::cin >> str; 1; std::cin >> str) {
		if (str == "y" || str == "Yes" || str == "yes" || str == "Y" || str == "YES") {
			for (int i = 1; i < k; ++i)
				delete_polynomial(polynomial_number - 2);
			B->print_base(B);
			break;
		} else if (str == "N" || str == "n" || str == "no" || str == "No" || str == "NO") {
			for(int i = 0; i < k; ++i)
				delete_polynomial(polynomial_number - 1);
			B->print_base(B);
			break;
		}
		std::cout << "Incorrect answer\ntry again" << std::endl;
	}
}

// Search for a polynomial in a database
std::string find(int a) {
	base *q = B;
	for (; q->n != a; q = q->next);
	return q->polynomialial();
}

// Sum 2 polynomials
void sum(int a, int c, int g = 1) {
	base *q = B;
	std::string str = find(a) + "+" + find(c);
	go(str, g);
}

// Reading a polynomial
void open(polynomial *&q, int n) {
	base *k = B;
	for (; k->n != n; k = k->next);
	q = k->p;
}

// Finding the derivative
void dx(int n, int k) {
	polynomial *q = new polynomial;
	open(q, n);
	std::string str, a, b;
	std::stringstream s;
	for (; q != nullptr; q = q->next, s.clear()) {
		str += "+";
		s << q->a * q->n;
		s >> a;
		s.clear();
		s << (q->n - 1);
		s >> b;
		if (q->n == 0) {
			b = a = "0";
		}
		str = str + a + "x" + b;
	}
	go(str, 0);
	for (int i = 1; i < k; ++i)
		dx(polynomial_number - 1, 1);
}

// Calcuting value at point
double f(int n, double x) {
	double result, k;
	polynomial *q = new polynomial;
	open(q, n);
	result = q->a;
	if (q->n == 0)
		return result;
	k = q->n - q->next->n;
	for (q = q->next; q != nullptr; q = q->next) {
		result = result * pow(x, k) + q->a;
		if (q->next != nullptr)
			k = q->n - q->next->n;
	}
	return result;
}

// Finding the root of a polynomial
void root(int n) {
	int z = 0;
	polynomial *q = new polynomial;
	open(q, n);
	if (find(n) == "0") {
		std::cout << "any x" << std::endl;
		return;
	}

	for (; q->next != nullptr && (q->next->n != 0 || q->next->a != 0); q = q->next);
	if (f(n, 0) == 0) {
		std::cout << 0 << " ";
		++z;
	}

	for (int i = 1; i <= abs(q->a); ++i) {
		if (q->a % i == 0) {
			if (f(n, i) == 0) {
				std::cout << " " << i << " ";
				++z;
			}
			if (f(n, -i) == 0) {
				std::cout << -i;
				++z;
			}
		}
	}

	if (z == 0)
		std::cout << "no roots";
	std::cout << std::endl;
}

// Product of two polynomials
void multiply(int n1, int n2, int g = 1) {
	std::string str, s1, s2;
	std::stringstream s;
	polynomial *a = new polynomial, *b = new polynomial;
	for (open(b, n2); b != nullptr; b = b->next)
		for (open(a, n1); a != nullptr; a = a->next, s.clear()) {
			str += "+";
			s << a->a * b->a;
			s >> s1;
			s.clear();
			s << a->n + b->n;
			s >> s2;
			str = str + s1 + "x" + s2;
		}
	go(str, g);
}

// Checking a polynomial for correctness
bool check(std::string str) {
	int i = 0, x = 0;
	if (isdigit(str[i]))
		goto num1;
	if (isalpha(str[i]))
		goto name;
	start:
		if (str[i] != '-' && str[i] != '+')
			return 0;
		for (; (str[i] == '+' || str[i] == '-') && i < str.size() - 1; ++i);
		if (isdigit(str[i]))
			goto num1;
		goto name;
	num1:
		if(i < str.size() - 1)
			if (str[i] == '0' && isdigit(str[i + 1]))
				return 0;
		for (; isdigit(str[i]) && i < str.size(); ++i);
		if (i == str.size())
			return 1;
		if (isalpha(str[i]) || str[i] == '*')
			goto name;
		goto start;
	name:
		if (str[i] == '*')
			++i;
		if (!isalpha(str[i]))
			return 0;
		if(!x)
			x = str[i];
		if (str[i] != x)
			return 0;
		if (i == str.size() - 1)
			return 1;
		++i;
		if (str[i] == '^' || isdigit(str[i]) && str[i] != '0')
			++i;
		if (str[i - 1] == '^' && i == str.size())
			return 0;
		if (str[i - 1] == '^' && !isdigit(str[i]))
			return 0;
		if (i < str.size() - 1)
			if (str[i] == '0' && isdigit(str[i + 1]))
				return 0;
		for (; i < str.size() && isdigit(str[i]); ++i);
		if (i == str.size())
			return 1;
		goto start;
}

// Display menu
void display_menu() {
	system("cls");
	std::cout << "\t\t\t\t\t\tMENU 2.0" << std::endl;
	std::cout << "Division:\t\"div A B\"" << std::endl;
	std::cout << "Addition:\t\"sum A B\"" << std::endl;
	std::cout << "Multiplication:\t\"mult A B\"" << std::endl;
	std::cout << "Integer roots:\t\"root A\"" << std::endl;
	std::cout << "Derivative: \t\"dx A B\""<< std::endl;
	std::cout << "Clear database: \t\t\"clear\"" << std::endl;
	std::cout << "Read entire file:\t\t\"download\"" << std::endl;
	std::cout << "Removing a polynomial:\t\t\"del N\"" << std::endl;
	std::cout << "Calculating a value at a point:\t\"f X\"" << std::endl;
	std::cout << "Read 1 polynomial from file: \t\"read\"" << std::endl;
	std::cout << "Display the menu again: \t\"menu\""<< std::endl;
}

// Division of two polynomials
bool divide(int a, int c) {
	std::string A, N, D;
	std::stringstream s;
	polynomial *q1 = new polynomial, *q2 = new polynomial;
	open(q1, a);
	open(q2, c);

	if (find(c) == "0")
		return 1;

	if (q1->a == 0)
		return 0;

	if (q1->a < q2->a || q1->n < q2->n) {
		if ((q1->a % q2->a != 0) || (q1->n - q2->n < 0)) {
			s << q1->n;
			s >> N;
			s.clear();
			s << q1->a;
			s >> A;
			remainder_str += "+" + A + "x" + N;
			go("-" + A + "x" + N, 0);
			sum(a, polynomial_number - 1, 0);
			divide(polynomial_number - 1, c);
			return 0;
		}
	}

	s << q1->n - q2->n;
	s >> N;
	s.clear();
	s << q1->a / q2->a;
	s >> A;
	result += "+" + A + "x" + N;
	go("-" + A + "x" + N, 0);
	multiply(c, polynomial_number - 1, 0);
	sum(a, polynomial_number - 1, 0);
	divide(polynomial_number - 1, c);
	return 0;
}
/**************************************************************************************************/
bool go(std::string str, int paint) {
	std::string a, c;
	std::transform(begin(str), end(str), begin(str), tolower);
	if (str != "end") {
		if (str == "print") {
			B->print_base(B);
		} else if (str == "delete" || str == "del") {
			std::cin >> a;
			if (!check_num(a))
				return 1;
			delete_polynomial(atoi(a.c_str()));
			B->print_base(B);
		} else if (str == "sum") {
			std::cin >> a >> c;
			if (!check_num(a) || !check_num(c))
				return 1;
			sum(atoi(a.c_str()), atoi(c.c_str()), 0);
			question(1, b->polynomialial());
		} else if (str == "dx") {
			base *u;
			std::cin >> a >> c;
			if (!check_num(a))
				return 1;
			if (atoi(c.c_str()) < 0) {
				std::cout << "Error" << std::endl;
				return 1;
			}
			for (u = B; u->n != atoi(a.c_str()); u = u->next);

			if (u->p->n < atoi(c.c_str()))
				go("0", 0);
			else if (atoi(c.c_str()) == 0)
				go(u->polynomialial(), 0);
			else
				dx(atoi(a.c_str()), atoi(c.c_str()));
			question(atoi(c.c_str()), b->polynomialial());
		} else if (str == "root" || str == "roots") {
			std::cin >> a;
			if (!check_num(a))
				return 1;
			root(atoi(a.c_str()));
		} else if (str == "f") {
			std::cin >> a >> c;
			if (!check_num(a))
				return 1;
			std::cout << f(atoi(a.c_str()), atof(c.c_str())) << std::endl;
		} else if (str == "mult") {
			std::cin >> a >> c;
			if (!check_num(a) || !check_num(c))
				return 1;
			multiply(atoi(a.c_str()), atoi(c.c_str()), 0);
			question(1, b->polynomialial());
		}
		else if (str == "clear") {
			for (; polynomial_number > 1; delete_polynomial(polynomial_number - 1));
			B->print_base(B);
		} else if (str == "menu") {
			display_menu();
		} else if (str == "read") {
			if (input >> str)
				go(str);
			else
				std::cout << "end of file" << std::endl;
		} else if (str == "download") {
			while (input >> str)
				go(str, 0);
			B->print_base(B);
		} else if (str == "div") {
			int d = polynomial_number;
			std::cin >> a >> c;
			if (!check_num(a) || !check_num(c))
				return 1;
			if (divide(atoi(a.c_str()), atoi(c.c_str())))
				std::cout << "Error" << std::endl;
			else {
				if (remainder_str != "0") {
					go(remainder_str, 0);
				}
				go(result, 0);
				std::cout << find(atoi(a.c_str())) + " = (" + find(polynomial_number - 1) + ")*(" + find(atoi(c.c_str())) + ")";
				if (find(polynomial_number - 2)[0] != '-' && remainder_str != "0")
					std::cout << " + ";
				if (remainder_str != "0")
					std::cout << find(polynomial_number - 2);
				question(polynomial_number - d," ",  0);
				result = remainder_str = "0";
			}
		} else if (!check(str)) {
			std::cout << "Error" << std::endl;
		} else {
			polynomial *p = new polynomial;
			new_polynomial(p, str);
			base *q = new base;
			q->p = p;
			b->next = q;
			b = b->next;
			b->n = polynomial_number++;
			if (paint) {
				B->print_base(B);
			}
		}
		return 1;
	}
	return 0;
}

int main() {
	std::string str;
	polynomial *z = new polynomial;
	B->p = z;
	display_menu();
	for (std::cin >> str; go(str); std::cin >> str);
	B->print_base(B);
    return 0;
}

