// concurrent.cpp : Defines the entry point for the console application.
//
//#include "stdafx.h"
#include <iostream>
#include <thread>
#include "vcf.h"
#include "removeduplicate.h"
#include "diploid.h"

using namespace std;

typedef struct Args {
	string ref_vcf_filename;
	string que_vcf_filename;
	string genome_seq_filename;
	string fp_filename;
	string fn_filename;
	string output_filename;
	bool direct_search;
	string chr_name;
	string stat_filename;
	int thread_num;
	bool remove_duplicates;
	string single_vcf_filename;
	bool match_genotype;
	bool normalization;
	bool score_basepair;
}Args;

bool ParserArgs(Args & args, int argc, char* argv[]) {
	//ez command parsing
	args.direct_search = false;
	args.remove_duplicates = false;
	args.match_genotype = true;
	args.normalization = false;
	args.score_basepair = false;
    args.thread_num = 1;
	for (int i = 1; i < argc; i++)
	{
		//cout << argv[i] << endl;
		if (!strcmp(argv[i], "-r")) {
			args.ref_vcf_filename = string(argv[++i]);
			if (!FileExists(args.ref_vcf_filename)) {
				cout << "[Error] ParserArgs: input reference vcf file not exist" << endl;
				return false;
			}
		}
		else if (!strcmp(argv[i], "-q")) {
			args.que_vcf_filename = string(argv[++i]);
			if (!FileExists(args.ref_vcf_filename)) {
				cout << "[Error] ParserArgs: input query vcf file not exist" << endl;
				return false;
			}
		}
		else if (!strcmp(argv[i], "-g")) {
			args.genome_seq_filename = string(argv[++i]);
			if (!FileExists(args.genome_seq_filename)) {
				cout << "[Error] ParserArgs: input genome sequence fasta file not exist" << endl;
				return false;
			}
		}
		else if (!strcmp(argv[i], "-p")) {
			args.fp_filename = string(argv[++i]);
		}
		else if (!strcmp(argv[i], "-n")) {
			args.fn_filename = string(argv[++i]);
		}
		else if (!strcmp(argv[i], "-o")) {
			args.output_filename = string(argv[++i]);
		}
		else if (!strcmp(argv[i], "-d")) {
			args.direct_search = true;
		}
		else if (!strcmp(argv[i], "-s")) {
			args.stat_filename = string(argv[++i]);
		}
		else if (!strcmp(argv[i], "-t")) {
			args.thread_num = atoi(argv[++i]);
		}
		else if (!strcmp(argv[i], "-m")) {
			args.remove_duplicates = true;
			args.single_vcf_filename = string(argv[++i]);
		}
		else if(!strcmp(argv[i], "-y")) {
			args.match_genotype = false;
		}
		else if (!strcmp(argv[i], "-b")) {
			args.score_basepair = true;
		}
		else if (!strcmp(argv[i], "-a")) {
			args.normalization = true;
		}
		else {
			cout << "[Error] Unrecognized parameter: " << argv[i] << endl;
			return false;
		}
	}
	return true;
	// end parsing command parameters ...
}

int usage(char* command) {
	cout << "\n";
	cout << "\tPlease cite our paper if you are using this program in your research." << endl;
    cout << endl;
	cout << "Usage: " << endl;
    cout << command << " -g genome file path(FASTA format)" << endl;
    cout << "\t-r reference VCF file path" << endl;
    cout << "\t-q query VCF file path" << endl;
    cout << "\t-o output filename prefix" << endl;
    cout << "\t[-t thread number]" << endl;
    cout << "\t[-d only perform direct search]" << endl;
    cout << "\t[-m single VCF file to remove duplicates]" << endl;
    cout << "\t[-y considering genotype when match vcf records]" << endl;
    cout << endl;

	return 0;
}

int main(int argc, char* argv[])
{
	dout << "Debug Mode" << endl;
	//DiploidVCF dvs(1);
	//dvs.test();
	//return 0;


	if (argc < 2) {
		usage(argv[0]);
		return 0;
	}
	Args args;
	if (!ParserArgs(args, argc, argv)) {
		usage(argv[0]);
		return 0;
	}

    if(args.remove_duplicates){
        RemoveDuplicate rd(args.thread_num);
        rd.Deduplicate(args.single_vcf_filename,
            args.genome_seq_filename,
            args.direct_search,
            args.output_filename);
        return 0;
	}

	DiploidVCF dv(1);
    dv.Compare(args.ref_vcf_filename,
		args.que_vcf_filename,
		args.genome_seq_filename,
		args.direct_search,
		args.output_filename,
		args.match_genotype,
		args.normalization,
		args.score_basepair);
	return 0;

	VCF vcf(args.thread_num);
	vcf.Compare(args.ref_vcf_filename,
			args.que_vcf_filename,
			args.genome_seq_filename,
			args.direct_search,
            args.output_filename,
            args.match_genotype,
			args.normalization);
    return 0;
}
