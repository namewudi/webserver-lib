#pragma once
namespace net{
    enum class Status{
        complete, parseingHeader, error, init, lineComplete, headerComplete
    };
}