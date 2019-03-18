//
//  Device.hpp
//  TTauri
//
//  Created by Tjienta Vara on 2019-02-06.
//  Copyright © 2019 Pokitec. All rights reserved.
//

#pragma once

#include "Queue.hpp"
#include "Window.hpp"

#include <unordered_set>

#include <boost/uuid/uuid.hpp>

#include <mutex>
#include <tuple>

namespace TTauri {
namespace GUI {

class Instance;


/*! A Device that handles a set of windows.
 */
class Device : public std::enable_shared_from_this<Device> {
public:
    enum class State {
        NO_DEVICE,
        READY_TO_DRAW,
    };

    std::recursive_mutex mutex;
    State state = State::NO_DEVICE;

    struct Error : virtual boost::exception, virtual std::exception {};
    
    std::string deviceName = "<no device>";
    uint32_t vendorID = 0;
    uint32_t deviceID = 0;
    boost::uuids::uuid deviceUUID;

    /*! A queue for sending graphics commands to.
     * These queue objects may be shared.
     *
     * ASSUMPTION: A single presentQueue can be used by all Windows on this Device.
     */
    std::shared_ptr<Queue> graphicQueue;

    /*! A queue for sending compute commands to.
     * These queue objects may be shared.
     *
     * ASSUMPTION: A single presentQueue can be used by all Windows on this Device.
     */
    std::shared_ptr<Queue> computeQueue;

    /*! A queue for sending present commands to.
     * These queue objects may be shared.
     *
     * ASSUMPTION: A single presentQueue can be used by all Windows on this Device.
     */
    std::shared_ptr<Queue> presentQueue;

    /*! A list of windows managed by this device.
     */
    std::unordered_set<std::shared_ptr<Window>> windows;

    std::string str() const;

    Device();
    virtual ~Device();

    /*! Check if this device is a good match for this window.
     *
     * It is possible for a window to be created that is not presentable, in case of a headless-virtual-display,
     * however in this case it may still be able to be displayed by any device.
     *
     * \returns -1 When not viable, 0 when not presentable, postive values for increasing score.
     */
    virtual int score(std::shared_ptr<Window> window) = 0;

    /*! Initialise the logical device.
     *
     * \param window is used as prototype for queue allocation.
     */
    virtual void initializeDevice(std::shared_ptr<Window> window);

    void add(std::shared_ptr<Window> window);

    void remove(std::shared_ptr<Window> window);

    /*! Refresh Display.
     *
     * \outTimestamp Number of nanoseconds since system start.
     * \outputTimestamp Number of nanoseconds since system start until the frame will be displayed on the screen.
     */
    void updateAndRender(uint64_t nowTimestamp, uint64_t outputTimestamp, bool blockOnVSync);

    /*! Maintanance work on low performance thread.
     */
    void maintance();

    
};

}}
