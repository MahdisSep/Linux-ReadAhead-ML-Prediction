#include <lttng/lttng.h>
#include <iostream>
#include <cstring>

int main() {
    lttng_event_field *field;
    lttng_event *event;
    lttng_trace_desc *trace;
    lttng_event_iter *iter;
    uint64_t inodeNumber;
    uint32_t flags;
    uint64_t timestamp;

    // Initialize LTTng
    if (lttng_init() != 0) {
        std::cerr << "Error initializing LTTng" << std::endl;
        return 1;
    }

    // Load the trace
    if (lttng_load_trace(&trace, "my_trace") != 0) {
        std::cerr << "Error loading trace" << std::endl;
        return 1;
    }

    // Find the event you are interested in
    //useful events="kvm_age_page,kvm_try_async_get_page,mm_vmscan_writepage,writeback_dirty_page,writeback_pages_written,writeback_wbc_writepage,writeback_balance_dirty_pages,writeback_dirty_inode_start,writeback_dirty_inode,writeback_mark_inode_dirty,writeback_write_inode_start,writeback_write_inode,writeback_sb_inodes_requeue,writeback_single_inode"
    event = lttng_event_by_name(trace, "my_event");
    if (!event) {
        std::cerr << "Error finding event" << std::endl;
        return 1;
    }

    // Create an event iterator
    iter = lttng_event_iter_create(event);
    if (!iter) {
        std::cerr << "Error creating event iterator" << std::endl;
        return 1;
    }

    // Iterate over the events
    while (lttng_event_iter_next(iter) == 0) {
        // Get the field that represents the inode number
        field = lttng_event_iter_get_field(iter, "inodeNumber");
        if (!field) {
            std::cerr << "Error getting inode number field" << std::endl;
            return 1;
        }
        // Get the inode number value
        if (lttng_event_field_integer_get_signed(field, &inodeNumber) != 0) {
            std::cerr << "Error getting inode number" << std::endl;
            return 1;
        }

        // Get the field that represents the flags
        field = lttng_event_iter_get_field(iter, "flags");
        if (!field) {
            std::cerr << "Error getting flags field" << std::endl;
            return 1;
        }
        // Get the flags value
        if (lttng_event_field_integer_get_unsigned(field, &flags) != 0) {
            std::cerr << "Error getting flags" << std::endl;
            return 1;
        }

        // Get the field that represents the timestamp
        field = lttng_event_iter_get_field(iter, "time");
        if (!field) {
            std::cerr << "Error getting timestamp field" << std::endl;
            return 1;
        }
        // Get the timestamp value
        if (lttng_event_field_integer_get_signed(field, &timestamp) != 0) {
            std::cerr << "Error getting timestamp" << std::endl;
            return 1;
        }

        // Process the extracted values
        std::cout << "Inode number: " << inodeNumber << std::endl;
        std::cout << "Flags: " << flags << std::endl;
        std::cout << "Timestamp: " << timestamp << std::endl;
    }

    // Clean up
    lttng_event_iter_destroy(iter);
    lttng_unload_trace(trace);
    lttng_exit();

    return 0;
}