/**
 * @file include/patterngen/pattern_extractor/pattern_extractor.h
 * @brief Binary pattern extractor.
 * @copyright (c) 2017 Avast Software, licensed under the MIT license
 */

#ifndef PATTERNGEN_PATTERN_EXTRACTOR_PATTERN_EXTRACTOR_H
#define PATTERNGEN_PATTERN_EXTRACTOR_PATTERN_EXTRACTOR_H

#include <ostream>
#include <string>
#include <vector>

#include "patterngen/pattern_extractor/types/symbol_pattern.h"

// Forward declarations.
namespace fileformat {
	class FileFormat;
	class Section;
	class Symbol;
} // namespace fileformat

namespace fnc_patterns {
namespace patterngen {

/**
 * Binary pattern extractor.
 */
class PatternExtractor
{
	private:
		std::unique_ptr<fileformat::FileFormat> inputFile; ///< Parser.

		bool stateValid = false;           ///< Extractor state.
		std::string errorMessage;          ///< Error message if invalid state.
		std::vector<std::string> warnings; ///< Vector with possible warnings.

		std::string groupName;               ///< Name for set of rules.
		std::vector<SymbolPattern> patterns; ///< Vector of patterns found.

		/// @brief Strange PIC32 architecture files processing.
		/// @{
		bool isPic32DataObjectOnlyFile();
		void processPic32DataObjectOnly();
		/// @}

		/// @brief Processing methods.
		/// @{
		bool processFile();
		bool checkPPC64Sections();
		std::vector<const fileformat::Symbol*> filterSymbols();
		void processSymbol(const fileformat::Symbol *symbol);
		void processSection(const fileformat::Section *section);
		void addSectionPatterns(
			const fileformat::Section *section,
			std::vector<const fileformat::Symbol *> &symbols);
		void addPattern(const fileformat::Section *section,
			const std::string &name, const unsigned long long offset,
			const unsigned long long size);
		std::string getArchAsString();
		/// @}

	public:
		/// @brief Constructors and destructor.
		/// @{
		PatternExtractor(const std::string &filePath,
			const std::string &groupName = "unknown_group");
		~PatternExtractor();
		/// @}

		/// @brief Error handling methods.
		/// @{
		bool isValid() const;
		std::string getErrorMessage() const;
		std::vector<std::string> getWarnings() const;
		/// @}

		/// @brief Output methods.
		/// @{
		void printRules(std::ostream &outputStream,
			const std::string &withNote = "") const;
		void addRulesToBuilder(yaramod::YaraFileBuilder &builder,
			const std::string &withNote = "") const;
		/// @}
};

} // namespace patterngen
} // namespace fnc_patterns

#endif
