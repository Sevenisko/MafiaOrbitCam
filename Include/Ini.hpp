#pragma once

#include <Utils.hpp>
#include <I3D_math.h>

class IniFile {
public:
    struct IniValue {
        template<typename T>
        T Get() const;

        void SetName(const std::string& szName) {
            this->szName = szName;
        }

        void Set(const std::string& szValue) {
            this->szValue = szValue;
        }

        void Set(int iValue) {
            this->szValue = std::to_string(iValue);
        }

        void Set(float fValue) {
            this->szValue = std::to_string(fValue);
        }

        void Set(bool bValue) {
            this->szValue = bValue ? "true" : "false";
        }

        void Set(const S_vector& vVec) {
            this->szValue = "{" + std::to_string(vVec.x) + ", " + std::to_string(vVec.y) + ", " + std::to_string(vVec.z) + "}";
        }

        bool IsValid() const {
            return bIsValid;
        }

    private:
        std::string szName;
        std::string szValue;
        bool bIsValid = false;

        friend class IniFile;
    };

    struct IniSection {
        bool HasValue(const std::string& szValueName) {
            for (auto& value : vValues) {
                if (value.szName == szValueName) {
                    return true;
                }
            }

            return false;
        }

        IniValue& operator[](const std::string& szName) {
            if (HasValue(szName))
                return GetValue(szName);
            else
                return AddValue(szName);
        }

        IniValue& AddValue(const std::string& szValueName) {
            IniValue value;
            value.bIsValid = true;
            value.szName   = szValueName;

            vValues.push_back(value);

            return vValues[vValues.size() - 1];
        }

        IniValue& GetValue(const std::string& szValueName) {
            for (auto& value : vValues) {
                if (value.szName == szValueName) {
                    return value;
                }
            }

            static IniValue invalidValue;
            invalidValue.bIsValid = false;

            return invalidValue;
        }
    private:
        std::string szName;
        std::vc6_vector<IniValue> vValues;

        friend class IniFile;
    };

    IniFile() {
        m_bIsValid = true;
    }

    IniFile(const std::string& szFileName) {
        std::ifstream file(szFileName);
        if (!file.is_open()) {
            return;
        }

        std::string line;
        IniSection* currentSection = &m_sRootSection;

        while (std::getline(file, line)) {
            line = Utils::TrimString(line);
            if (line.empty() || line[0] == ';' || line[0] == '#') {
                continue; // Skip empty lines and comments
            }

            if (line[0] == '[' && line.back() == ']') {
                IniSection newSection;
                newSection.szName = line.substr(1, line.size() - 2);
                m_vSections.push_back(newSection);
                currentSection = &m_vSections.back();
            } else {
                size_t delimiterPos = line.find_first_of("=");
                if (delimiterPos != std::string::npos) {
                    IniValue value;
                    value.szName   = Utils::TrimString(line.substr(0, delimiterPos));
                    value.szValue  = Utils::TrimString(line.substr(delimiterPos + 1));
                    value.bIsValid = true;
                    if (currentSection != nullptr) {
                        currentSection->vValues.push_back(value);
                    }
                }
            }
        }

        file.close();
        m_bIsValid = true;
    }

    void Write(const std::string& szFileName) {
        std::ofstream file(szFileName);

        for (auto& value : m_sRootSection.vValues) {
            file << value.szName + " = " + value.szValue << std::endl;
        }
        file << std::endl;

        for (auto& section : m_vSections) {
            file << "[" + section.szName + "]" << std::endl;
            for (auto& value : section.vValues) {
                file << value.szName + " = " + value.szValue << std::endl;
            }
            file << std::endl;
        }
    }

    bool IsValid() const {
        return m_bIsValid;
    }

    bool HasSection(const std::string& szSectionName) {
        for (auto& section : m_vSections) {
            if (section.szName == szSectionName) {
                return true;
            }
        }

        return false;
    }

    IniSection& operator[](const std::string& szName) {
        if (HasSection(szName))
            return GetSection(szName);
        else if (szName == "#root")
            return m_sRootSection;
        else
            return AddSection(szName);
    }

    IniSection& AddSection(const std::string& szSectionName) {
        IniSection section;
        section.szName = szSectionName;

        m_vSections.push_back(section);

        return m_vSections[m_vSections.size() - 1];
    }

    IniSection& GetSection(const std::string& szSectionName) {
        for (auto& section : m_vSections) {
            if (section.szName == szSectionName) {
                return section;
            }
        }

        return m_sRootSection;
    }

private:
    IniSection m_sRootSection;
    std::vc6_vector<IniSection> m_vSections;
    bool m_bIsValid = false;
};

template <>
inline int IniFile::IniValue::Get<int>() const {
    return std::stoi(szValue);
}

template <>
inline float IniFile::IniValue::Get<float>() const {
    return std::stof(szValue);
}

template <>
inline bool IniFile::IniValue::Get<bool>() const {
    if (szValue == "true") {
        return true;
    }

    return false;
}

template <>
inline std::string IniFile::IniValue::Get<std::string>() const {
    return szValue;
}

template <>
inline S_vector IniFile::IniValue::Get<S_vector>() const {
    S_vector vec;
    std::string str = szValue;

    if (str.front() == '{') {
        str.erase(str.begin());
    }
    if (str.back() == '}') {
        str.pop_back();
    }

    str.erase(std::remove_if(str.begin(), str.end(), ::isspace), str.end());

    std::istringstream iss(str);
    char delimiter;

    if (!((iss >> vec.x >> delimiter) && (delimiter == ',') && (iss >> vec.y >> delimiter) && (delimiter == ',') && (iss >> vec.z))) {
        vec = {};
    }

    return vec;
}