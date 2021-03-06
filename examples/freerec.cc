/*! \include freerec.cc
*/

/*
  freerec.cc 
  Simulates a population bottleneck model with independent sites
  and total mutation rate theta=4Nu
*/

#include <Sequence/Coalescent/Coalescent.hpp>
#include <ctime>
#include <limits>
#include <iostream>
#include <cstdio>
#include <stdexcept>
#include <cassert>
#include <numeric>
#include <random>
using namespace std;

//need to define these
namespace Sequence {
  namespace coalsim {
    MAX_SEG_T MAX_SEGSITES=200;
    MAX_SEG_T MAX_SEGS_INC=200;
  }
}

//overload of the libsequence function in the global namespace
//this version allows you to determine the site at which
//the marginal tree begins
//(this overload is unambiguous, as the libsequence version
//takes a single argument)
Sequence::coalsim::marginal init_marginal( const int & nsam , const int & begin)
{
  std::vector<Sequence::coalsim::node> tree(2*nsam-1);
  for(int i=0;i<nsam;++i)
    {
      tree[i] = Sequence::coalsim::node(0.);
    }
  return Sequence::coalsim::marginal(begin,nsam,nsam-1,tree);
}

int main(int argc, char ** argv)
{
  if(argc<8)
    {
      exit(10);
    }
  const int nsam = atoi(argv[1]);     //sample size
  const double theta = atof(argv[2]); //4Nu
  int nruns = atoi(argv[3]);          //how many replicates to simulate
  int nsites = atoi(argv[4]);         //how many independent sites 
  const double tr = atof(argv[5]);    //time in past (in units of 4Ne gens) 
                                      //at which the population
                                      //recovered from the bottleneck
  const double d = atof(argv[6]);     //duration of bottleneck (units of 4Ne gens)
  const double f = atof(argv[7]);     //severity of bneck ( 0 < f <= 1)

  //some sanity checks on parameters
  if( tr<0.||d<0.||f<0. )
    {
      exit(10);
    }
  else if (tr>0.&&d>=0.&&f<=0)
    {
      exit(10);
    }

  unsigned seed = std::numeric_limits<unsigned>::max();
  if(argc==9)
    seed=atoi(argv[8]);

  std::mt19937 generator(seed);

  //Make our RNG types via lambda expressions
  auto poiss = [&generator](const double & mean){ return std::poisson_distribution<int>(mean)(generator); };
  auto expo = [&generator](const double & mean){ return std::exponential_distribution<double>(1/mean)(generator); };
  auto uni01 = [&generator](){ return std::uniform_real_distribution<double>(0.,1.)(generator); };
  auto uni = [&generator](const double & a, const double & b){ return std::uniform_real_distribution<double>(a,b)(generator); };

  const double DMAX = std::numeric_limits<double>::max();
  vector<Sequence::coalsim::chromosome> initialized_sample = 
    Sequence::coalsim::init_sample( vector<int>(1,nsam),nsites );


  for(int i=0;i<argc;++i)
    {
      cout << argv[i] << ' ';
    }
  cout << '\n';

  Sequence::coalsim::gamete_storage_type gametes( std::vector<double>(Sequence::coalsim::MAX_SEGSITES,0.),
			       std::vector<std::string>(nsam,
							std::string(Sequence::coalsim::MAX_SEGSITES,'0')) );
  int segsites;
  unsigned MAXCH=0;
  bool event = false;

  try
    {
      while(nruns--)
	{
	  //we will collect nsites independently-simulated
	  //trees in the list independent_histories
	  Sequence::coalsim::arg independent_histories;

	  for(int site=0;site<nsites;++site)
	    {
	      vector<Sequence::coalsim::chromosome> sample;
	      sample.reserve(MAXCH);
	      std::copy(initialized_sample.begin(),
			initialized_sample.end(),
			std::back_inserter(sample));
	      int NSAM = nsam;
	      Sequence::coalsim::arg sample_history(1,init_marginal(nsam,site));
	      int nlinks = nsam;
	      double t = 0.;
	      while(NSAM > 1)
		{
		  double rel_size = ( t < tr || t >= (tr+d) ) ? 1. : f;
		  double rcoal = double(NSAM*(NSAM-1));
		  double tcoal = expo(rel_size/rcoal);
		  if ( t<tr && t+tcoal >= tr )
		    {
		      t = tr;
		      event=false;
		    }
		  else if (t < (tr+d) && t+tcoal >= (tr+d) )
		    {
		      t = tr+d;
		      event = false;
		    }
		  else event = true;
		  if(event)
		    {
		      t+=tcoal;
		      pair<int,int> two = Sequence::coalsim::pick2(uni,NSAM);
		      NSAM -= Sequence::coalsim::coalesce(t,nsam,NSAM,two.first,two.second,nsites,
				       &nlinks,&sample,&sample_history);
		      //clean up some uncessary storage
		      if (NSAM < int(sample.size())/5)
			{
			  sample.erase(sample.begin()+NSAM+1,sample.end());
			}
		    }
		}
	      //we've just simulated 1 tree, which we add to the end of 
	      //indepent_histories. This builds up an "ARG" of independent
	      //trees.
	      //we need to dereference the pointer (iterator) to the
	      //first tree in sample_history, since arg types have no
	      //"bracket" (operator[]) access
	      independent_histories.push_back(*(sample_history.begin()));
	      sample.clear();
	    }
	  segsites = Sequence::coalsim::infinite_sites(poiss,uni,&gametes,
					      nsites,independent_histories,theta);
	  Sequence::coalsim::output_gametes(stdout,segsites,nsam,gametes);
	  independent_histories.clear();
	}
    }
  catch (std::exception & e)
    {
      std::cerr << "exception caught: " << e.what() << std::endl;
      exit(1);
    }
}
