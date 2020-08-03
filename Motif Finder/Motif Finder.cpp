#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <sstream>
using namespace std;

class Sequence
{
	string name;
	string wholeSequence;
	vector<int> quality;
	vector<char> seq;
public:
	// Name
	void setName(string _name)
	{
		name = _name;
	}
	string getName()
	{
		return name;
	}

	// Sequence
	void setSequence(string sequence)
	{
		wholeSequence = sequence;
	}
	void addSequence()
	{
		copy(wholeSequence.begin(), wholeSequence.end(), back_inserter(seq));
	}
	char getSequence(int num)
	{
		return seq[num];
	}
	int getSequenceSize()
	{
		return seq.size();
	}
	string getWholeSequence()
	{
		return wholeSequence;
	}

	// Quality
	void addQuality(int num)
	{
		quality.push_back(num);
	}
	int getQuality(int num)
	{
		return quality[num];
	}
	int getQualitySize()
	{
		return quality.size();
	}
};
int totalSequenceSize(Sequence arr[])
{
	int total = 0;
	for (int i = 0; i < 7; i++)
		total = total + arr[i].getSequenceSize();
	return total;
}
void loadFile(Sequence arr[])
{
	ifstream in;
	string line, line2;
	string val;
	string str;

	//Wczytanie pliku fasta
	in.open("fasta.txt");
	if (in.is_open())
	{
		cout << "Loaded Fasta file" << endl;
		for (int i = 0; i < 7; i++)
		{
			getline(in, line);
			arr[i].setName(line);
			getline(in, line);
			arr[i].setSequence(line + line2);
			arr[i].addSequence();
		}
	}
	else
	{
		cout << "		!!!Error while loading the FASTA file!!!" << endl;
		exit(0);
	}
	in.close();

	//Wczytanie pliku quality
	in.open("quality.txt");
	if (in.is_open())
	{
		cout << "Loaded Quality file" << endl;
		for (int i = 0; i < 7; i++)
		{
			getline(in, line);
			getline(in, line);
			istringstream ss(line);
			int num;
			while (ss >> num)
			{
				arr[i].addQuality(num);
			}
		}
	}
	else
	{
		cout << "		!!!Error while loading the QUALITY file!!!" << endl;
		exit(0);
	}
}
void printEverything(Sequence arr[])
{
	for (int i = 0; i < 7; i++)
	{
		cout << "Sequence " << i << endl;
		cout << " name :" << arr[i].getName() << endl;
		//cout << " seq  :" << arr[i].getWholeSequence() << endl;
		cout << " size of qual:  " << arr[i].getQualitySize() << "  size of seq:  " << arr[i].getSequenceSize() << endl;
		cout << " val  :";
		for (size_t j = 0; j < arr[i].getQualitySize(); j++)
			cout << arr[i].getQuality(j) << " ";
		cout << endl;
		cout << " sequence  :";
		for (size_t k = 0; k < arr[i].getSequenceSize(); k++)
			cout << arr[i].getSequence(k);
		cout << endl << endl;
	}
	cout << "Total sum of nucleotides: " << totalSequenceSize(arr) << endl;

}


class Vertice
{
public:
	int number;
	int startPos;
	int origin; //0-6
	bool visited;
	vector<int> qual;
	vector<char> seq;
	vector<int> next;
	char getSequence(int num)
	{
		return seq[num];
	}
	int getQuality(int num)
	{
		return qual[num];
	}
	int getOrigin()
	{
		return origin;
	}
	int getNextSize()
	{
		return next.size();
	}
	void printSequence()
	{
		for (int i = 0; i < seq.size(); i++)
			cout << seq[i];
	}
};

void printVertices(vector<Vertice> graph, int substring)
{
	vector<Vertice>::iterator it;
	int v = 0;
	for (it = graph.begin(); it != graph.end(); ++it)
	{
		cout << "vertice: " << it->number << endl;
		cout << "origin: " << it->origin << endl;
		cout << "start position: " << it->startPos << endl;
		cout << "sequence: ";
		for (size_t k = 0; k < substring; k++)
			cout << it->seq[k];

		cout << endl << "next: ";
		for (int l = 0; l < graph[v].getNextSize(); l++)
			cout << graph[v].next[l] << " ";
		cout << endl << endl;
		v++;
	}
}

void fillEdges(vector<Vertice>& graph, int substring, int reliability, int count)
{
	int pass = 0;
	int del = 0;
	int possibleDel = 1;
	if (substring > 5)
		possibleDel = 2;
	for (int a = 0; a < count; a++)
	{
		for (int b = a + 1; b < count; b++)
		{
			if (graph[a].origin != graph[b].origin)
			{
				for (int i = 0; i < substring; i++)
				{
					if (graph[a].seq[i] == graph[b].seq[i])
					{
						pass++;
					}
					else if ((graph[a].qual[i] < reliability || graph[b].qual[i] < reliability))
					{
						del++;
						pass++;
					}
				}
				if (del <= possibleDel && pass == substring)
				{
					graph[a].next.push_back(b);
					graph[b].next.push_back(a);
				}
				pass = 0;
				del = 0;
			}
		}
	}
}

void visitNode(vector<Vertice> graph, int v, vector<Vertice>& temp, vector<Vertice>& best, bool visitedSeq[], bool& key)
{
	if (key == true)
		return;
	graph[v].visited = true;
	visitedSeq[graph[v].origin] = true;
	temp.push_back(graph[v]);
	if (temp.size() > best.size())
		best = temp;
	if (best.size() == 7)
	{
		key = true;
		return;
	}
	for (int i = 0; i < graph[v].getNextSize(); i++)
	{
		if (visitedSeq[graph[graph[v].next[i]].getOrigin()] == false && graph[graph[v].next[i]].visited == false)
			visitNode(graph, graph[v].next[i], temp, best, visitedSeq, key);
	}
	temp.pop_back();
	visitedSeq[graph[v].origin] = false;
}
vector<Vertice> findClique(vector<Vertice> graph)
{
	vector<Vertice> temp;
	vector<Vertice> best;
	bool visitedSeq[7] = {};
	bool key = false;
	for (int i = 0; i < graph.size(); i++)
		graph[i].visited = false;
	for (int j = 0; j < graph.size(); j++)
		visitNode(graph, j, temp, best, visitedSeq, key);

	return best;
}

int main()
{
	Sequence arr[7];
	int substring;
	int reliability;
	loadFile(arr);
	printEverything(arr);
	cout << endl;
	do
	{
		cout << "Enter k-mer size (4-7) " << endl;
		cin >> substring;
		if (substring < 4 || substring > 7)
		{
			cout << "Wrong k-mer size, enter a value between 4 and 7: " << endl;
			cin >> substring;
		}
	} while (substring < 4 || substring > 7);

	do
	{
		cout << "Enter reliability threshold (10-40) " << endl;
		cin >> reliability;
		if (reliability < 10 || reliability > 40)
		{
			cout << "Wrong reliability threshold, enter a value between 10 and 40: " << endl;
			cin >> substring;;
		}
	} while (reliability < 10 || reliability > 40);

	vector<Vertice> graph;
	int total = 0;
	for (int i = 0; i < 7; i++) //dla kazdej sekwencji z tablicy
	{
		for (int j = 0; j < arr[i].getSequenceSize() - substring + 1; j++) // dla kazdego nukleotydu 
		{
			Vertice* v = new Vertice;
			for (int k = 0; k < substring; k++) //dodaj nukleotyd plus tyle ile substring
			{
				v->number = total;
				v->seq.push_back(arr[i].getSequence(j + k));
				v->qual.push_back(arr[i].getQuality(j + k));
				v->origin = i;
				v->startPos = j;
			}
			graph.push_back(*v);
			total++;
		}
	}


	fillEdges(graph, substring, reliability, total);
	printVertices(graph, substring);
	vector<Vertice> best = findClique(graph);
	cout << "Size of the best found clique: " << best.size() << endl;
	for (int j = 0; j < best.size(); j++)
	{
		cout << " seq ";
		best[j].printSequence();
		cout << " origin " << best[j].origin << endl;
	}
}