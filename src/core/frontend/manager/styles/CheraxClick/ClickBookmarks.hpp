#pragma once
#include <filesystem>
#include <string>
#include <vector>
namespace YimMenu::ClickUI
{
    struct Bookmark { std::string Submenu, Category; };
    class Bookmarks
    {
    public:
        bool Load(const std::filesystem::path& file);
        bool Toggle(const std::string& submenu, const std::string& category);
        bool Contains(const std::string& submenu, const std::string& category) const;
        const std::vector<Bookmark>& All() const { return m_Items; }
        const std::string& Error() const { return m_Error; }
    private:
        std::filesystem::path m_File;
        std::vector<Bookmark> m_Items;
        std::string m_Error;
    };
}
