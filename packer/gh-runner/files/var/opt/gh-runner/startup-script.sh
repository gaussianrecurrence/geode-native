#!/usr/bin/env bash

# Licensed to the Apache Software Foundation (ASF) under one or more
# contributor license agreements.  See the NOTICE file distributed with
# this work for additional information regarding copyright ownership.
# The ASF licenses this file to You under the Apache License, Version 2.0
# (the "License"); you may not use this file except in compliance with
# the License.  You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

set -e -o pipefail

function print_help()
{
  >&2 echo "
  $(basename "$0") [options]

      --help            Shows this command help.

      --token           Specifies the registration token for the runner.

      --repo            Specifies the repository name.

      --labels          Specifies a set of labels separated by comma.
  "
}

TOKEN=""
LABELS=""
REPOSITORY=""
WORK_PATH="/var/opt/gh-runner"

while [[ $# -gt 0 ]]
do
  key="$1"

  case $key in
    --labels)
      shift
      LABELS="$1"
      shift
      ;;
    --token)
      shift
      TOKEN="$1"
      shift
      ;;
    --repo)
      shift
      REPOSITORY="$1"
      shift
      ;;
    --help)
      shift
      print_help
      exit 1
      ;;
    *)
      shift
      ;;
  esac
done

cd "${WORK_PATH}"

>2& echo "[INFO] Setting up permissions..."
chown -R github:github "${WORK_PATH}"

>2& echo "[INFO] Downloading runner..."
GH_RUNNER_URL=$(curl -s -X GET --url "https://api.github.com/repos/actions/runner/releases/latest" \
                     -H "Accept: application/vnd.github.v3+json" \
                     -H "Content-Type: application/json" --fail | \
jq -r '.assets[] | select(.name | contains("linux-x64")) | .browser_download_url')

su github -c "curl -s -L ${GH_RUNNER_URL} | tar xzf -"
unset GH_RUNNER_URL

>2& echo "[INFO] Setting up GitHub runner..."
su github -c "./config.sh --url \"https://github.com/${REPOSITORY}\" --labels \"${LABELS}\" --token \"${TOKEN}\" --unattended"

>2& echo "[INFO] Installing GitHub runner service..."
./svc.sh install github

>2& echo "[INFO] Starting GitHub runner service..."
./svc.sh start
