/* Copyright 2019 Kristofer Björnson
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/** @file HartreeFock.cpp
 *
 *  @author Kristofer Björnson
 */

#include "TBTK/Solver/HartreeFock.h"
#include "TBTK/UnitHandler.h"
#include "TBTK/KineticOperator.h"
#include "TBTK/HartreeFockPotentialOperator.h"
#include "TBTK/NuclearPotentialOperator.h"

using namespace std;

namespace TBTK{
namespace Solver{

HartreeFock::HartreeFock(){
	densityMatrix = Matrix<complex<double>>(4, 4);
}

HartreeFock::~HartreeFock(){
}

double HartreeFock::getTotalEnergy(){
	complex<double> amplitude = 0;

	const BasisStateSet &basisStateSet = getModel().getBasisStateSet();
	const HoppingAmplitudeSet &hoppingAmplitudeSet
		= getModel().getHoppingAmplitudeSet();

	for(
		BasisStateSet::ConstIterator iterator0
			= basisStateSet.cbegin();
		iterator0 != basisStateSet.cend();
		++iterator0
	){
		unsigned int linearIndex0 = hoppingAmplitudeSet.getBasisIndex(
			(*iterator0).getIndex()
		);

		for(
			BasisStateSet::ConstIterator iterator1
				= basisStateSet.cbegin();
			iterator1 != basisStateSet.cend();
			++iterator1
		){
			unsigned int linearIndex1
				= hoppingAmplitudeSet.getBasisIndex(
					(*iterator1).getIndex()
				);

			const AbstractState &braState = *iterator0;
			const AbstractState &ketState = *iterator1;

			//Kinetic term.
			amplitude += densityMatrix.at(
				linearIndex0,
				linearIndex1
			)*braState.getMatrixElement(
				ketState,
				KineticOperator(UnitHandler::getM_eN())
			);

			//Hartree-Fock term.
			for(
				BasisStateSet::ConstIterator iterator2
					= basisStateSet.cbegin();
				iterator2 != basisStateSet.cend();
				++iterator2
			){
				unsigned int linearIndex2
					= hoppingAmplitudeSet.getBasisIndex(
						(*iterator2).getIndex()
					);

				for(
					BasisStateSet::ConstIterator iterator3
						= basisStateSet.cbegin();
					iterator3 != basisStateSet.cend();
					++iterator3
				){
					unsigned int linearIndex3
						= hoppingAmplitudeSet.getBasisIndex(
							(*iterator3).getIndex()
						);

					amplitude += densityMatrix.at(
						linearIndex0,
						linearIndex1
					)*densityMatrix.at(
						linearIndex2,
						linearIndex3
					)*braState.getMatrixElement(
						ketState,
						HartreeFockPotentialOperator(
							*iterator2,
							*iterator3
						)
					)/2.;
				}
			}

			//Nuclear potential term.
			for(
				unsigned int n = 0;
				n < nuclearCenters.size();
				n++
			){
				amplitude += densityMatrix.at(
					linearIndex0,
					linearIndex1
				)*braState.getMatrixElement(
					ketState,
					NuclearPotentialOperator(
						nuclearCenters[n],
						nuclearCenters[n].getPosition()
					)
				);
			}
		}
	}

	//Intra-nuclear potential energy.
	double e = UnitHandler::getEN();
	double epsilon_0 = UnitHandler::getEpsilon_0N();
	double prefactor = pow(e, 2)/(4*M_PI*epsilon_0);
	for(unsigned int m = 0; m < nuclearCenters.size(); m++){
		for(unsigned int n = 0; n < nuclearCenters.size(); n++){
			if(m == n)
				continue;

			double z0 = nuclearCenters[m].getAtomicNumber();
			double z1 = nuclearCenters[n].getAtomicNumber();
			double r = (
				nuclearCenters[m].getPosition()
				- nuclearCenters[n].getPosition()
			).norm();

			amplitude += (1/2.)*prefactor*z0*z1/r;
		}
	}

	return real(amplitude);
}

HartreeFock::Callbacks::Callbacks(){
	solver = nullptr;
}

complex<double> HartreeFock::Callbacks::getHoppingAmplitude(
	const Index &to,
	const Index &from
) const{
	TBTKAssert(
		solver != nullptr,
		"Solver::HartreeFock::Callbacks::getHoppingAmplitude()",
		"Solver not set.",
		"Use Solver::HartreeFock::Callbacks::setSolver() to set the"
		<< " Solver that the Callbacks is associate with."
	);

	complex<double> amplitude = 0;
	const Model &model = solver->getModel();
	const Matrix<complex<double>> &densityMatrix
		= solver->getDensityMatrix();
	const vector<PositionedAtom> nuclearCenters
		= solver->getNuclearCenters();

	const BasisStateSet &basisStateSet = model.getBasisStateSet();
	const AbstractState &braState = basisStateSet.get(to);
	const AbstractState &ketState = basisStateSet.get(from);

	//Kinetic term.
	amplitude += braState.getMatrixElement(
		ketState,
		KineticOperator(UnitHandler::getM_eN())
	);

	//Hartree-Fock potential.
	for(
		BasisStateSet::ConstIterator iterator0
			= basisStateSet.cbegin();
		iterator0 != basisStateSet.cend();
		++iterator0
	){
		unsigned int linearIndex0
			= model.getHoppingAmplitudeSet().getBasisIndex(
				(*iterator0).getIndex()
			);

		for(
			BasisStateSet::ConstIterator iterator1
				= basisStateSet.cbegin();
			iterator1 != basisStateSet.cend();
			++iterator1
		){
			unsigned int linearIndex1
				= model.getHoppingAmplitudeSet().getBasisIndex(
					(*iterator1).getIndex()
				);

			amplitude += densityMatrix.at(
				linearIndex0,
				linearIndex1
			)*braState.getMatrixElement(
				ketState,
				HartreeFockPotentialOperator(
					*iterator0,
					*iterator1
				)
			);
		}
	}

	//Nuclear potential term.
	for(unsigned int n = 0; n < nuclearCenters.size(); n++){
		amplitude += braState.getMatrixElement(
			ketState,
			NuclearPotentialOperator(
				nuclearCenters[n],
				nuclearCenters[n].getPosition()
			)
		);
	}

	return amplitude;
}

complex<double> HartreeFock::Callbacks::getOverlapAmplitude(
	const Index &bra,
	const Index &ket
) const{
	TBTKAssert(
		solver != nullptr,
		"Solver::HartreeFock::Callbacks::getOverlapAmplitude()",
		"Solver not set.",
		"Use Solver::HartreeFock::Callbacks::setSolver() to set the"
		<< " Solver that the Callbacks is associate with."
	);

	const BasisStateSet &basisStateSet
		= solver->getModel().getBasisStateSet();

	return basisStateSet.get(bra).getOverlap(basisStateSet.get(ket));
}

};	//End of namespace Solver
};	//End of namespace TBTK