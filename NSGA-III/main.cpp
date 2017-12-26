
#include "problem_base.h"
#include "alg_nsgaiii.h"
#include "alg_population.h"
#include "exp_experiment.h"

#include <ctime>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <thread>

#include "gnuplot_interface.h"
#include "log.h"
#include "aux_misc.h"
#include "exp_indicator.h"

using namespace std;

int main() {
	ifstream exp_list("explist.ini");
	if (!exp_list) { cout << "We need the explist.ini file." << endl; return 1; }

	string exp_name;
	while (exp_list >> exp_name)
	{
		ifstream exp_ini("Experiments\\" + exp_name);
		if (!exp_ini) { cout << exp_name << " file does not exist." << endl; continue; }

		// ----- Setup the expriment ------
		CNSGAIII nsgaiii;
		BProblem *problem = 0;

		SetupExperiment(nsgaiii, &problem, exp_ini);
		Gnuplot gplot, gplot2;
		ofstream IGD_results(nsgaiii.name() + "-" + problem->name() + "-IGD.txt"); // output file for IGD values per run

		// affichage du front de pareto pour chaque solution
		string PF_results("PF/"+problem->name() + "-PF.txt"); // output file for PF values per experiment
		string scale = "1:2";
		std::size_t found = problem->name().find("(");
		if (found != std::string::npos) {
			std::size_t f = problem->name().find(")");
			scale = problem->name().substr(found+1, f - found-1);
			string s = "";
			// here à changer pour afficher le front de pareto
			for (int i = 1; i < stoi(scale); i++)
			{
				s += i + ":";
			}
			s += scale;
			cout << s;
			scale = s;

		}
		gplot.set_title("Pareto");
		gplot2("plot \"" + PF_results + "\" using " + scale);

		

		// ----- Run the algorithm to solve the designated function -----
		const size_t NumRuns = 20; // 20 is the setting in NSGA-III paper
		for (size_t r=0; r<NumRuns; r+=1)
		{
			srand(r); cout << "Solving " << problem->name() << " ... Run: " << r << endl;

			// --- Solve
			CPopulation solutions;		
			nsgaiii.Solve(&solutions, *problem);

			// --- Output the result
			string logfname = "Results\\" + nsgaiii.name() + "-" + problem->name() + "-Run" + IntToStr(r) + ".txt"; // e.g. NSGAIII-DTLZ1(3)-Run0.txt
			SaveScatterData(logfname, solutions); 
		
			// --- Calculate the performance metric
			TFront PF, approximation;
			IGD_results << IGD(LoadFront(PF, "PF\\"+ problem->name() + "-PF.txt"), LoadFront(approximation, logfname)) << endl;
		
			// --- Visualization (Show the last 3 dimensions. You need gnuplot.)
			ShowPopulation(gplot, solutions, "gnuplot-show"); 
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		system("pause");

		delete problem;
		
	}// while - there are more experiments to carry out

	return 0;
}
