#include "ClickBookmarks.hpp"
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <stdexcept>
#include <sstream>
namespace YimMenu::ClickUI
{
    bool Bookmarks::Contains(const std::string& submenu, const std::string& category) const
    {
        return std::any_of(m_Items.begin(), m_Items.end(), [&](const auto& b) { return b.Submenu == submenu && b.Category == category; });
    }
    bool Bookmarks::Load(const std::filesystem::path& file)
    {
        m_File = file;
        try
        {
            if (!std::filesystem::exists(file)) return true;
            if (std::filesystem::file_size(file) > 32768) throw std::runtime_error("Bookmark file is too large.");
            std::ifstream input(file);
            if (!input) throw std::runtime_error("Cannot read bookmarks.");
            std::vector<Bookmark> candidate;
            std::string a, b;
            std::string line;
            while (std::getline(input,line))
            {
                if (line.empty()) continue;
                std::istringstream row(line); std::string extra;
                if (!(row >> std::quoted(a) >> std::quoted(b)) || (row >> extra)) throw std::runtime_error("Invalid bookmark file.");
                if (a.size() > 160 || b.size() > 160 || a.find_first_of("\r\n") != std::string::npos || b.find_first_of("\r\n") != std::string::npos)
                    throw std::runtime_error("Invalid bookmark entry.");
                if (std::none_of(candidate.begin(),candidate.end(),[&](const auto& item) { return item.Submenu==a && item.Category==b; })) candidate.push_back({a,b});
            }
            if (input.bad()) throw std::runtime_error("Cannot read bookmark file.");
            m_Items = std::move(candidate); m_Error.clear(); return true;
        }
        catch (const std::exception& e) { m_Error = e.what(); return false; }
    }
    bool Bookmarks::Toggle(const std::string& submenu, const std::string& category)
    {
        const auto previous = m_Items;
        if (Contains(submenu, category))
            m_Items.erase(std::remove_if(m_Items.begin(), m_Items.end(), [&](const auto& b) { return b.Submenu == submenu && b.Category == category; }), m_Items.end());
        else m_Items.push_back({submenu, category});
        try
        {
            if (m_File.empty()) throw std::runtime_error("Bookmark folder is unavailable.");
            std::ofstream output(m_File, std::ios::trunc);
            for (const auto& b : m_Items) output << std::quoted(b.Submenu) << ' ' << std::quoted(b.Category) << '\n';
            output.flush(); if (!output) throw std::runtime_error("Cannot save bookmarks.");
            m_Error.clear(); return true;
        }
        catch (const std::exception& e) { m_Items = previous; m_Error = e.what(); return false; }
    }
}
