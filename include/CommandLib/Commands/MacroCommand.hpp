#pragma once

#include "../ICommand.hpp"
#include <CommandLib/Commands/SharedICommandSerialization.hpp>
#include <boost/serialization/access.hpp>
#include <boost/serialization/nvp.hpp>
#include <vector>

class MacroCommand : public ICommand {
public:
    void Add(std::shared_ptr<ICommand> command);
    void Execute(IObject* object) override;
    std::vector<uint8_t> Serialize() const override;
    std::string Type() const override;

    std::vector<std::shared_ptr<ICommand>> GetCommands() const;

private:
    std::vector<std::shared_ptr<ICommand>> commands_;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive& ar, const unsigned int) {
        ar & BOOST_SERIALIZATION_NVP(commands_);
    }
};
