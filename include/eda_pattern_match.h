/*
 * This program source code file is part of KiCad, a free EDA CAD application.
 *
 * Copyright (C) 2015 Chris Pavlina <pavlina.chris@gmail.com>
 * Copyright (C) 2015 KiCad Developers, see change_log.txt for contributors.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, you may find one here:
 * http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
 * or you may search the http://www.gnu.org website for the version 2 license,
 * or you may write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

/**
 * @file eda_pattern_match.h
 * @brief Abstract pattern-matching tool and implementations.
 */

#ifndef EDA_PATTERN_MATCH_H
#define EDA_PATTERN_MATCH_H

#include <vector>
#include <wx/wx.h>
#include <wx/string.h>
#include <wx/regex.h>

static const int EDA_PATTERN_NOT_FOUND = wxNOT_FOUND;

class EDA_PATTERN_MATCH_SUBSTR;
class EDA_PATTERN_MATCH_REGEX;
class EDA_PATTERN_MATCH_WILDCARD;

class EDA_PATTERN_MATCH
{
public:
    /**
     * Set the pattern against which candidates will be matched.
     */
    virtual void SetPattern( const wxString& aPattern ) = 0;

    /**
     * Return the location of a match iff a given candidate string matches the set pattern.
     * Otherwise, return EDA_PATTERN_NOT_FOUND.
     */
    virtual int Find( const wxString& aCandidate ) = 0;

    /**
     * Populate a list of (non-translated) pattern matcher IDs
     */
    static void GetPatternMatcherIds( std::vector<wxString>& aList );

    /**
     * Get the translated name for a pattern matcher ID.
     */
    static wxString GetPatternMatcherName( const wxString& aId );

    /**
     * Return a new pattern matcher given an ID.
     */
    static EDA_PATTERN_MATCH* CreatePatternMatcher( const wxString& aID );
};


class EDA_PATTERN_MATCH_SUBSTR : public EDA_PATTERN_MATCH
{
public:
    /**
     * Construct an EDA_PATTERN_MATCH_SUBSTR
     */
    EDA_PATTERN_MATCH_SUBSTR() {}

    /**
     * Set the pattern against which candidates will be matched.
     */
    virtual void SetPattern( const wxString& aPattern );

    /**
     * Return the location of a match iff a given candidate string contains the set
     * substring. Otherwise, return EDA_PATTERN_NOT_FOUND.
     */
    virtual int Find( const wxString& aCandidate );

protected:
    wxString m_pattern;
};


class EDA_PATTERN_MATCH_REGEX : public EDA_PATTERN_MATCH
{
public:
    /**
     * Construct an EDA_PATTERN_MATCH_REGEX
     */
    EDA_PATTERN_MATCH_REGEX() {}

    /**
     * Set the pattern against which candidates will be matched.
     */
    virtual void SetPattern( const wxString& aPattern );

    /**
     * Return the location of a match iff a given candidate string contains the set
     * substring with wildcards. Otherwise, return EDA_PATTERN_NOT_FOUND.
     */
    virtual int Find( const wxString& aCandidate );

protected:
    wxString m_pattern;
    wxRegEx m_regex;
};


class EDA_PATTERN_MATCH_WILDCARD : public EDA_PATTERN_MATCH_REGEX
{
public:
    /**
     * Construct an EDA_PATTERN_MATCH_WILDCARD
     */
    EDA_PATTERN_MATCH_WILDCARD() {}

    /**
     * Set the pattern against which candidates will be matched.
     */
    virtual void SetPattern( const wxString& aPattern );

    /**
     * Return the location of a match iff a given candidate string contains the set
     * substring with wildcards. Otherwise, return EDA_PATTERN_NOT_FOUND.
     */
    virtual int Find( const wxString& aCandidate );
};

#endif  // EDA_PATTERN_MATCH_H
