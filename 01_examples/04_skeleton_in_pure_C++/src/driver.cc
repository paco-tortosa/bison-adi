/* Driver for calc++.   -*- C++ -*-

   Copyright (C) 2005-2015, 2018-2021 Free Software Foundation, Inc.

   This file is part of Bison, the GNU Compiler Compiler.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

#include "driver.hh"
#include "parser.hh"

extern int yy_flex_debug;
typedef struct yy_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE yy_scan_string ( const char *yy_str  );

int driver::m_Parse (const std::string& _strOperations) {
    std::string strBuffer("Buffer");
    m_cLocation.initialize(&strBuffer);

    //Scan begin
    yy_flex_debug = m_bTraceScanning;
    yy_scan_string(_strOperations.c_str());

    //Parse
    yy::parser parse (*this);
    parse.set_debug_level(m_bTraceParsing);
    int res = parse.parse();
    
    //Result
    std::cout << "Result = " << m_iResult;

    return res;
}
