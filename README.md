# tdb

A Silly Program For Tracking Tea Inventory and Consumption

tdb exists to play with the mysql c api, in addition to having a little fun managing my caffeine collection.


# setup

* libmysqlclient
* libreadline6

Copy `config.example.lua` to `config.lua` and put in the correct credentials.
`db-setup.sql` has schema and some sample db values.


# license

tdb is copyright (C) 2014 - 2016  David Ulrich.

For full license details see LICENSE.

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Affero General Public License as published
by the Free Software Foundation, version 3 of the License.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Affero General Public License for more details.

You should have received a copy of the GNU Affero General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
