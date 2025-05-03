#include <CommandLib/Commands/MoveCommand.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <sstream>

MoveCommand::MoveCommand(const std::array<double, 2> &delta)
    : delta_(delta)
{}

void MoveCommand::Execute(IObject* object) {
    if (!object->HasProperty("position")) return;

    auto posData = object->GetProperty("position");
    double* position = reinterpret_cast<double*>(posData.data());
    position[0] += delta_[0];
    position[1] += delta_[1];

    object->SetProperty("position", std::vector<uint8_t>(
        reinterpret_cast<uint8_t*>(position), reinterpret_cast<uint8_t*>(position + 2)));
}

std::vector<uint8_t> MoveCommand::Serialize() const {
    std::ostringstream oss;
    boost::archive::binary_oarchive oa(oss);
    oa << Type();
    oa << *this;
    return {oss.str().begin(), oss.str().end()};
}

std::string MoveCommand::Type() const {
    return "MoveCommand";
}

namespace {
bool registered = []() -> bool {
    ICommand::RegisterCommand("MoveCommand", [](const uint8_t* data, size_t size, IObject* context) {
        std::istringstream iss(std::string(reinterpret_cast<const char*>(data), size));
        boost::archive::binary_iarchive iar(iss);
        std::string type;
        iar >> type;
        MoveCommand cmd(std::array<double, 2>{0, 0});
        iar >> cmd;
        return std::make_shared<MoveCommand>(cmd);
    });
    return true;
}();
}
