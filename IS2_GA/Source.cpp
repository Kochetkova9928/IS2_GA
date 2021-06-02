#include <iostream> // для cout и т.п.
#include <vector> // для класса vector
#include <string> // для класса string
#include <algorithm> // для алгоритма сортировки
#include <time.h> // для случайных величин
#include <math.h> // для abs()

#define GA_SIZE 3000 // размер популяции
#define GA_MAXITER 20000 // максимальное число итераций
#define GA_BEST 0.2f // лучшие
#define GA_MUT 0.5f // мутации
#define GA_MUTATION RAND_MAX * GA_MUT
#define GA_TARGET std::string("МИР")
const std::string GA_TABLE = " абвгдеёжзийклмнопрстуфхцчшщъыьэюяАБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ";

using namespace std;

struct ga_struct
{
	string str; // строка
	unsigned int rating; // пригодность
};

// формирование первой (начальной) популяции
void first_population(vector<ga_struct>& cur_population, vector<ga_struct>& next_population) {
	int target_size = GA_TARGET.size();

	for (int i = 0; i < GA_SIZE; i++) {
		ga_struct chromosoma;

		chromosoma.rating = 0;
		chromosoma.str.erase();

		// формиирование хромосом
		for (int j = 0; j < target_size; j++) {
			chromosoma.str += GA_TABLE[(rand() % GA_TABLE.size())];
		}

		cur_population.push_back(chromosoma);
	}

	next_population.resize(GA_SIZE);
}

// оценка популяции (пригодность)
void calc_rating(vector<ga_struct>& population) {
	string target = GA_TARGET;
	int target_size = target.size();
	unsigned int rating;

	for (int i = 0; i < GA_SIZE; i++) {
		rating = 0;
		for (int j = 0; j < target_size; j++) {
			// Округляем результат до целого для ускорения вычислений
			rating += abs(int(population[i].str[j] - target[j]));
		}

		population[i].rating = rating;
	}
}

// компаратор для сравнения популяций
bool sort_by_rating_comp(ga_struct a, ga_struct b) {
	return (a.rating < b.rating);
}

// сортировка популяций по оценке
inline void sort_by_rating(vector<ga_struct>& population) {
	sort(population.begin(), population.end(), sort_by_rating_comp);
}

// селекция
void selection(vector<ga_struct>& cur_population, vector<ga_struct>& next_population, int best_size) {
	for (int i = 0; i < best_size; i++) {
		next_population[i].str = cur_population[i].str;
		next_population[i].rating = cur_population[i].rating;
	}
}

// мутация
void mutate(ga_struct& member) {
	int target_size = GA_TARGET.size();
	int ipos = rand() % target_size;

	member.str[ipos] = GA_TABLE[(rand() % GA_TABLE.size())];
}

// селекция, скрещивание, мутация
void mate(vector<ga_struct>& cur_population, vector<ga_struct>& next_population) {
	int best_size = GA_SIZE * GA_BEST;
	int target_size = GA_TARGET.size();
	int rand_target;
	int p1;
	int p2;

	// селекция
	selection(cur_population, next_population, best_size);


	for (int i = best_size; i < GA_SIZE; i++) {
		p1 = rand() % (GA_SIZE / 2);
		p2 = rand() % (GA_SIZE / 2);
		rand_target = rand() % target_size;

		// скрещивание
		next_population[i].str = cur_population[p1].str.substr(0, rand_target) +
			cur_population[p2].str.substr(rand_target, best_size - rand_target);

		// матация
		if (rand() < GA_MUTATION) {
			mutate(next_population[i]);
		}
	}
}

// вывод лучшей популяции
inline void print_best(vector<ga_struct>& pop) {
	cout << pop[0].str << " (" << pop[0].rating << ")" << endl;
}

// очистка буффера
inline void swap(vector<ga_struct>*& cur_population, vector<ga_struct>*& next_population) {
	vector<ga_struct>* temp = cur_population;
	cur_population = next_population;
	next_population = temp;
}

int main() {
	srand(unsigned(time(NULL)));
	setlocale(LC_ALL, "Russian");
	system("chcp 1251");

	vector<ga_struct> pop_a, pop_b;
	vector<ga_struct>* cur_population, * next_population;

	first_population(pop_a, pop_b); // формирование первой (начальной) популяции
	cur_population = &pop_a;
	next_population = &pop_b;

	for (int i = 0; i < GA_MAXITER; i++) {
		calc_rating(*cur_population); // оцениваем популяцию
		sort_by_rating(*cur_population); // сортируем популяцию
		print_best(*cur_population); // выводим лучшую популяцию

		if ((*cur_population)[0].rating == 0) {
			break;
		}

		mate(*cur_population, *next_population); // спариваем популяции
		swap(cur_population, next_population); // очищаем буферы
	}

	return 0;
}
