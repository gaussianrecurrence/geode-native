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



//#include "../../../ICacheLoader.hpp"
#include "../ICacheLoader.hpp"
//#include "../Serializable.hpp"
#include "../Region.hpp"
#include "SafeConvert.hpp"
//#include "../legacy/impl/SafeConvert.hpp"
//#include "../../../Region.hpp"
//#include "../../../Cache.hpp"

using namespace System;

//using namespace Apache::Geode::Client;

namespace Apache
{
  namespace Geode
  {
    namespace Client
    {

      public interface class ICacheLoaderProxy
      {
      public:
        std::shared_ptr<apache::geode::client::Cacheable> load(apache::geode::client::Region& region,
          const std::shared_ptr<apache::geode::client::CacheableKey>& key, const std::shared_ptr<apache::geode::client::Serializable>& helper );

        void close(apache::geode::client::Region& region );
      };

      generic<class TKey, class TValue>
      public ref class CacheLoaderGeneric : ICacheLoaderProxy // : Apache::Geode::Client::ICacheLoader /*<Object^, Object^>*/
      {
        private:

          ICacheLoader<TKey, TValue>^ m_loader;

        public:

          void SetCacheLoader(ICacheLoader<TKey, TValue>^ loader)
          {
            m_loader = loader;
          }

          virtual std::shared_ptr<apache::geode::client::Cacheable> load( apache::geode::client::Region& region,
            const std::shared_ptr<apache::geode::client::CacheableKey>& key, const std::shared_ptr<apache::geode::client::Serializable>& helper )
          {
            IRegion<TKey, TValue>^ gregion = Region<TKey, TValue>::Create(&region);

            TKey gkey = TypeRegistry::GetManagedValueGeneric<TKey>(key);

            Object^ ghelper = TypeRegistry::GetManagedValueGeneric<Object^>(helper);

            return Serializable::GetUnmanagedValueGeneric<TValue>(m_loader->Load(gregion, gkey, ghelper));
          }

          virtual void close(apache::geode::client::Region& region )
          {
            IRegion<TKey, TValue>^ gregion = Region<TKey, TValue>::Create(&region);
            m_loader->Close(gregion);
          }
      };
    }  // namespace Client
  }  // namespace Geode
}  // namespace Apache

