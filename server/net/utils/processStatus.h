#pragma once
namespace net{
    enum class Status{
        complete, hasMore, error, init, lineComplete, headerComplete
    };
}