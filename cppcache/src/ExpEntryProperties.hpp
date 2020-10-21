/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#ifndef GEODE_EXPENTRYPROPERTIES_H_
#define GEODE_EXPENTRYPROPERTIES_H_

#include <atomic>
#include <memory>

#include <geode/CacheableKey.hpp>
#include <geode/internal/geode_globals.hpp>

#include "ExpiryTaskManager.hpp"

namespace apache {
namespace geode {
namespace client {
/**
 * @brief This class encapsulates expiration specific properties for
 *        a MapEntry.
 */
class APACHE_GEODE_EXPORT ExpEntryProperties {
 public:
  using time_point = std::chrono::steady_clock::time_point;

  explicit ExpEntryProperties(ExpiryTaskManager* manager) : manager_{manager} {}

  time_point last_accessed() const { return last_accessed_; }

  time_point last_modified() const { return last_modified_; }

  void last_accessed(const time_point& tp) { last_accessed_ = tp; }

  void last_modified(const time_point& tp) { last_modified_ = tp; }

  void task_id(ExpiryTask::id_t task_id) { task_id_ = task_id; }

  bool task_scheduled() const { return task_id_ != ExpiryTask::invalid(); }

  void cancel_task() const { manager_->cancel(task_id_); }

 protected:
  // this constructor deliberately skips initializing any fields
  inline explicit ExpEntryProperties(bool) {}

 private:
  /** last access time in secs, 32bit.. */
  std::atomic<time_point> last_accessed_{time_point{}};
  /** last modified time in secs, 32bit.. */
  std::atomic<time_point> last_modified_{time_point{}};

  ExpiryTaskManager* manager_{nullptr};

  /** The expiry task id for this particular entry.. **/
  ExpiryTask::id_t task_id_{ExpiryTask::invalid()};
};
}  // namespace client
}  // namespace geode
}  // namespace apache

#endif  // GEODE_EXPENTRYPROPERTIES_H_
