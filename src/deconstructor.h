/**********************************************************************
deconstructor.h - Deconstruct a MOF into its building blocks and simplified net
***********************************************************************/

#ifndef DECONSTRUCTOR_H
#define DECONSTRUCTOR_H

#include <string>

#include <openbabel/babelconfig.h>
#include <openbabel/generic.h>
#include <openbabel/obconversion.h>

#include "topology.h"

namespace OpenBabel
{
// forward declarations
class OBMol;


// Default directory for CIF/Systre outputs.  Also used in Python/extract_moffles.py and
const std::string DEFAULT_OUTPUT_PATH = "Output/";


std::string writeFragments(std::vector<OBMol> fragments, OBConversion obconv);
std::string getSMILES(OBMol fragment, OBConversion obconv);


class Deconstructor {
// Base class for MOF deconstruction algorithms, to go from an OBMol of original atoms
// to a simplified net, its topology, and the mapping of net pseudoatoms back to the MOF.
private:  // hidden from derived classes, too
	std::string output_dir;

protected:
	OBMol *parent_molp;
	Topology simplified_net;
	OBConversion obconv;
	bool infinite_node_detected;
	virtual void InitOutputFormat();

	static std::string GetBasicSMILES(OBMol fragment);

	// Network simplification steps
	virtual void DetectInitialNodesAndLinkers();
	virtual bool CollapseNodes();
	virtual void SimplifyTopology();
	virtual void PostSimplification() {};
	int CheckCatenation();
	std::string GetCatenationInfo(int num_nets);

public:
	Deconstructor(OBMol* orig_mof);
	virtual ~Deconstructor() {};

	void SimplifyMOF(bool write_intermediate_cifs=true);

	// Output CIFs and building block identity.
	void SetOutputDir(const std::string &path);
	virtual void WriteCIFs();
	virtual std::string GetMOFInfo();

	// Utilities
	std::string GetOutputPath(const std::string &filename);
	void WriteSimplifiedNet(const std::string &base_filename);
	void WriteAtomsOfRole(const std::string &simplified_role, const std::string &base_filename="");
};


class MOFidDeconstructor : public Deconstructor {
// The original MOFid algorithm and implementation of Deconstructor (originally in sbu.cpp).
// Converts 4-c linkers in MIL-47, etc., to 2 x 3-c.
protected:
	virtual void PostSimplification();

public:
	MOFidDeconstructor(OBMol* orig_mof = NULL);
	virtual ~MOFidDeconstructor() {};
};


//typedef Deconstructor MOFidDeconstructor;  // if it was implemented as an alias
// Not yet implemented:
//class AllNodeDeconstructor : public MOFidDeconstructor {};
//class SingleNodeDeconstructor : public MOFidDeconstructor {};


} // end namespace OpenBabel
#endif // DECONSTRUCTOR_H

//! \file deconstructor.h
//! \brief deconstructor.h - Deconstruct a MOF into its building blocks and simplified net