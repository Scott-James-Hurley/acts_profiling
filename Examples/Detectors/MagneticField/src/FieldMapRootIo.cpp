// This file is part of the Acts project.
//
// Copyright (C) 2017-2021 CERN for the benefit of the Acts project
//
// This Source Code Form is subject to the terms of the Mozilla Public
// License, v. 2.0. If a copy of the MPL was not distributed with this
// file, You can obtain one at http://mozilla.org/MPL/2.0/.

#include "ActsExamples/MagneticField/FieldMapRootIo.hpp"

#include "Acts/MagneticField/BFieldMapUtils.hpp"

#include <vector>

#include <TFile.h>
#include <TTree.h>

ActsExamples::detail::InterpolatedMagneticField2
ActsExamples::makeMagneticFieldMapRzFromRoot(
    std::function<size_t(std::array<size_t, 2> binsRZ,
                         std::array<size_t, 2> nBinsRZ)>
        localToGlobalBin,
    std::string fieldMapFile, std::string treeName, Acts::ActsScalar lengthUnit,
    Acts::ActsScalar BFieldUnit, bool firstQuadrant) {
  /// [1] Read in field map file
  // Grid position points in r and z
  std::vector<double> rPos;
  std::vector<double> zPos;
  // components of magnetic field on grid points
  std::vector<Acts::Vector2> bField;
  // [1] Read in file and fill values
  TFile* inputFile = TFile::Open(fieldMapFile.c_str());
  TTree* tree = (TTree*)inputFile->Get(treeName.c_str());
  Int_t entries = tree->GetEntries();

  double r, z;
  double Br, Bz;

  tree->SetBranchAddress("r", &r);
  tree->SetBranchAddress("z", &z);

  tree->SetBranchAddress("Br", &Br);
  tree->SetBranchAddress("Bz", &Bz);

  // reserve size
  rPos.reserve(entries);
  zPos.reserve(entries);
  bField.reserve(entries);

  for (int i = 0; i < entries; i++) {
    tree->GetEvent(i);
    rPos.push_back(r);
    zPos.push_back(z);
    bField.push_back(Acts::Vector2(Br, Bz));
  }
  inputFile->Close();
  /// [2] use helper function in core
  return Acts::fieldMapRZ(localToGlobalBin, rPos, zPos, bField, lengthUnit,
                          BFieldUnit, firstQuadrant);
}

ActsExamples::detail::InterpolatedMagneticField3
ActsExamples::makeMagneticFieldMapXyzFromRoot(
    std::function<size_t(std::array<size_t, 3> binsXYZ,
                         std::array<size_t, 3> nBinsXYZ)>
        localToGlobalBin,
    std::string fieldMapFile, std::string treeName, Acts::ActsScalar lengthUnit,
    Acts::ActsScalar BFieldUnit, bool firstOctant) {
  /// [1] Read in field map file
  // Grid position points in x, y and z
  std::vector<double> xPos;
  std::vector<double> yPos;
  std::vector<double> zPos;
  // components of magnetic field on grid points
  std::vector<Acts::Vector3> bField;
  // [1] Read in file and fill values
  TFile* inputFile = TFile::Open(fieldMapFile.c_str());
  TTree* tree = (TTree*)inputFile->Get(treeName.c_str());
  Int_t entries = tree->GetEntries();

  double x, y, z;
  double Bx, By, Bz;

  tree->SetBranchAddress("x", &x);
  tree->SetBranchAddress("y", &y);
  tree->SetBranchAddress("z", &z);

  tree->SetBranchAddress("Bx", &Bx);
  tree->SetBranchAddress("By", &By);
  tree->SetBranchAddress("Bz", &Bz);

  // reserve size
  xPos.reserve(entries);
  yPos.reserve(entries);
  zPos.reserve(entries);
  bField.reserve(entries);

  for (int i = 0; i < entries; i++) {
    tree->GetEvent(i);
    xPos.push_back(x);
    yPos.push_back(y);
    zPos.push_back(z);
    bField.push_back(Acts::Vector3(Bx, By, Bz));
  }
  inputFile->Close();

  return Acts::fieldMapXYZ(localToGlobalBin, xPos, yPos, zPos, bField,
                           lengthUnit, BFieldUnit, firstOctant);
}
