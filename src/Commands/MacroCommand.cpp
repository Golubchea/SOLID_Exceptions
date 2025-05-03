#include <CommandLib/Commands/MacroCommand.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>

void MacroCommand::Add(std::shared_ptr<ICommand> command) {
    commands_.push_back(command);
}

void MacroCommand::Execute(IObject* object) {
    for (auto& cmd : commands_) {
        cmd->Execute(object);
    }
}

std::vector<uint8_t> MacroCommand::Serialize() const {
    std::ostringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << Type();
    oa << commands_;
    return {oss.str().begin(), oss.str().end()};
}

std::string MacroCommand::Type() const {
    return "MacroCommand";
}

std::vector<std::shared_ptr<ICommand>> MacroCommand::GetCommands() const {
    return commands_;
}

namespace {
bool registered = []() -> bool {
    ICommand::RegisterCommand("MacroCommand", [](const uint8_t* data, size_t size, IObject* context) {
        std::istringstream iss(std::string(reinterpret_cast<const char*>(data), size));
        boost::archive::binary_iarchive iar(iss);
        std::string type;
        iar >> type;
        MacroCommand cmd;
        iar >> cmd;
        return std::make_shared<MacroCommand>(cmd);
    });
    return true;
}();
}
