Ebay Pricing System
-	Ebay searchByImage API: https://developer.ebay.com/api-docs/buy/browse/resources/item_summary/methods/searchByImage
-	Google Vision AI (https://cloud.google.com/vision) send results to Google Shopping
o	Was thinking may be able to send results to ChatGPT, but doesn’t seem like this is super effective
Could train a model based on pricing information, however, would require much more engineering than just a simple algorithmic approach. Would require webscraping and such to get enough data for price prediction. It is possible though. 
Selenium is an automatable web browser that you can configure to allow for a robot to scrape data from the web or for unit testing.
One can find webscraping loop holes: https://oxylabs.io/blog/is-web-scraping-legal . Upon investigation I was thinking maybe it would be possible for me to develop a browser plugin that would pay individual users for the transactions that they make, be it shopping or selling and then the idea is not that we are scraping data off the web page, but that we are paying individual users for their purchase data. We could develop an application that would allow individual’s to sell us their purchase history on websites?
Idea of having a tool which goes around collecting all of your data in a basic and understandable format and then allows you to sell it is pretty good.

Where can performance be gained on ebay selling? Measuring and weighing should not take that long. Accurate pricing and finding similar products is non-trivially automatable. It would require hooking into a variety of eCommerce APIs and trying to collect data that way. In order to do image recognition you would need to collect the product pages from many online retailers. In Europe it seems that it is legal to buy data from consumers as they have the right to data transportability: https://europa.eu/youreurope/citizens/consumers/internet-telecoms/data-protection-online-privacy/index_en.htm . Consequently, a product that collects your data and allows you to sell and manage it may be pretty freaking cool.
Another thing that you wanted to do was recycleable detection. It seems that these guys are already looking into doing that: https://techcrunch.com/2022/08/12/trashbot-uses-ai-to-sort-recyclables/ . 
I keep getting the feeling that there are many engineering applications which we have the compute and the AI to do, we just need the data. I think I should focus on allowing consumers to sell their data as well as use it in a more intelligent manner.
I think the technology to look in to is personal data collection. Can focus on all browser site data. I am having difficulty finding such a personal data scraper out there. Can also focus on developing applications which then use this data locally for interesting things like Clay. You should pick one application of personal data collection that would be helpful to you, and then enhance your plugin to become more extensive, then create an online data marketplace, and then keep going from there. We require COMPLETE access to all your data, but our guarantee is that we don’t do anything with it. My experience shows that adding in special security measures and having different levels of accessibility is unconvincing to people. Can prompt users in certain directions to acquire more interesting and useful data. You need to make this extension open source so that people trust it. Then you need to do cool and interesting things with the collected data and this will be the most compelling to people.

Interesting things that I can do with personal data:
-	Monitor online interactions and personal relationships
-	Track calendar and get more helpful calendar tips
-	Get more helpful browser habit tips
-	Get more helpful financial tips
-	Facebook birthdays
Features:
-	Have a password to encrypt collected data
-	Best to not collect passwords
-	Have data mixing, so sell the combined anonymized data of 10 different people so that it is very difficult to track which data belongs to which people? The unmixed data is very valuable though.
-	Will be good for creating aggregated insights. 
-	Can have an app or something in the future, but for now chrome extension is a good place to start.
-	Have a rigidly defined data format, so that other people can use other tools with this data format and then you can connect various tools and applications to the data via this format.

Grocery Interest Calculator:
-	Get receipts from email
-	Get order history from online
Data analysis application that is able to track this over time
Geared towards personal usage, so categories will be developed to divide personal appropriately

Personal data collection system
-	Personal data can then be easily used and accessed by other applications for consumer benefit
-	Base level application housing data should be free and should be able to interface with other extensions which can be installed as apps on devices or on browser
-	Should have a local or online storage option, but start with local
-	Extensions collect a wide variety of personal data which is collected in this application
-	Another application set of data analysis or model training applications can be installed on the other end of the data management system and allow users to intelligently use their personal data
-	Data cannot be sold since it is very difficult to ensure its security and its integrity
    o	Could devise some system of plausibility checking, but best to avoid for now
-	I believe that web scraping is legal if it is going on in your customary use and if you are the one authorizing the data collection.
- Could have an application that users can use to interface with the data storage system and install extensions and watch them run or something, instead of accessing it as a completely headless library 

We are going to do this. Think of your cool application and let’s get jumping. No questions asked, I am just going to do it. I will do it all in rust and webassmebly. 

Database structure type:
-	I want a database that is going to be able to many values whose schemas can vary
-	Options (https://www.mongodb.com/databases/types ):
o	Hierarchical
	Tree structured database
	E.g. File System
o	Relational
	Use SQL for creating, reading, updating, and deleting (CRUD)
	Stores data in discrete tables that can then be joined together for later processing
o	Non-relational
	NoSQL databases
	Have grown increasingly popular
	Scale better than relational for large datasets
	Graph Database
•	Data points are seen as nodes in the graph with many connections and relationships between nodes
o	Object Oriented Databases
	Store object’s on server’s disk
	Associations between objects can persist
	Easy interface with object oriented programming
-	It seems based on the list above that I am going to want a NoSQL database, and ChatGPT agrees
-	I don’t want a document or graph database, so this leaves Key Value store and Column-oriented databases
-	I don’t want a column-oriented database, since I want my data structured into rows, therefore I am going to opt for a key value store database
-	Key Value (non-relational) database: https://aws.amazon.com/nosql/key-value/ 

ChatGPT NoSQL Database Options:
-	AWS Dynamo DB
-	Google Cloud Platform Cloud Datastore
-	Microsoft Azure Cosmos DB
-	Couchbase
-	Redis
-	RocksDB
-	Lightning Memory-Mapped Database
ChatGPT said RocksDB is generally used for databases in the TB to several TB range
ChatGPT Database Memory ranking (high uses a lot of memory)
1.	Redis: Redis is an in-memory data store, which means all data is stored in memory, leading to higher memory usage compared to databases that store data on disk. It offers various data structures and built-in compression, but overall, it can have the highest memory usage among the mentioned databases.
2.	Amazon DynamoDB Local: The memory usage of DynamoDB Local can vary depending on the configuration settings, such as the provisioned read and write capacity. It can have a relatively higher memory footprint compared to some of the other databases.
3.	Google Cloud Datastore Emulator: Like DynamoDB Local, the memory usage of Cloud Datastore Emulator depends on the configuration settings and workload. Its memory footprint can be relatively higher than some other databases on this list.
4.	Azure Cosmos DB Emulator: Similar to the previous two, Azure Cosmos DB Emulator's memory usage depends on the configuration settings and workload. Its memory footprint can be relatively higher than other databases on this list.
5.	Couchbase: Couchbase uses both memory and disk for data storage, and its memory usage depends on the configuration settings such as caching, indexing, and replication. It offers built-in compression, which can help reduce memory usage.
6.	RocksDB: RocksDB is an LSM (Log-Structured Merge) tree-based storage engine optimized for SSDs and fast storage. It uses memory for caching and buffering, but the majority of the data is stored on disk. The memory usage depends on the configuration settings, such as block cache size and write buffer size. It can have lower memory usage compared to some other databases on this list.
7.	LMDB: LMDB is a memory-mapped key-value store, which means the operating system manages memory usage. It maps the entire database file to memory but doesn't load it all at once. Instead, it loads pages on-demand, resulting in lower memory usage compared to some other databases.
I am going to use RocksDB, since my projected use case is that you have the data stored on your local SSD or external SSD and it seems optimized for data storage on an SSD.

5/8/2023

## Motivation:
 Develop a local application that provides a consistent interface to data collection plugins and data analysis and machine learning plugins

## Features:
- open source so that people are able to trust the management system and the plugins
- incredible documentation so that it is easy for people to set things up and understand the project organization
- clearly defined interfaces
- simple so that it is easy for people to understand
- be able to very easily import as a library with a very basic interface on a wide variety of computer programs and applications
- backup and primary storage location
- databases grow seperately and then be merged together or synced together
- mobile application
- Could potentially support multiple database types based on nature of how information is being written and queried across them, and then provide a cross database interface for all of them
- Can have storage frequency limitations. Less frequently data is written the more storage allowed or something like this.
- Archive ability to offload collected data onto external source
- Can have different interface for different interprocess communication systems if you would like to speed up the database read and write time
- Have a feature for importing different data categories?
- You can have a personal database store and an impersonal big data store that can be housed in two different locations. Impersonal big data store may just be data which was scraped from a site that you got access too after getting through their login wall. e.g. personal would be your linked in friends, impersonal would be all linkedIn members from an organization you are a part of. Because you are part of this organization you can see all these members, but this information is not necessarily specific to you.

## Security Features:
- plugins are only able to edit certain data segments
    - for starters probably best to say that anyone can write anything, but no one can delete

## Plan:
- Implement a very base level of the system
- Use it on the grocery application

## Interface:
- import library
- [POST] register_client(name, password) -> app
    - password makes actually no sense if you want it to just be a wrapper and not a service or anything
- [POST] create_group(category, name, array of inputs that are keyed by) -> group
    - Can really build this out to take in certain data types and how the prefix filter should treat each of these and if it even needs a prefix filter
- [POST] group.put(array of inputs keyed by, value)
- [POST] group.get(array of inputs keyed by)
    - Will need to have special considerations here for ranges and such things
- [GET] list_categories()
- [GET] list_clients()
- [POST] connect_buddy(path)
- [POST] create_buddy(path)

## Future Interface:
- enriched to allow for database configuration
- allows for creation of multiple databases and configuring the database location
- can support merging of multiple instances

## RockDB Notes:
- Source: https://github.com/facebook/rocksdb/wiki/RocksDB-Overview 
- Basic Interface:
    - Get(key)
    - NewIterator()
    - Put(key, val)
    - Delete(key, val)
    - SingleDete(key)
- Writes are put into the memtable and may be written to the log if they aren't able to just be immediately written to the disk
- Column Families:
    - https://github.com/facebook/rocksdb/wiki/Column-Families
    - patitions a database
    - guarantees consistent view across column families
    - supports cross family operations
    - mechanism for grouping data, just as how data is grouped into tables in a relational database
    - each key value associated with exactly 1 column family
    - 
- All data is stored in sorted order
- Supports transactions and has different modes for optimistic and pessimistic transactions
- Can scan through key values, based on a specific prefix given to the key and can do this quite efficiently
- Different compaction styles
- Can delete whole datafiles
- Has read only mode
- Has a Time to Live feature allowing data to expire
- Uses a LRU cache, but can also switch this to a clock cache for block eviction
    - A LRU cache approximates a LFU cache, so the thing that is used least recently is evicted
- Can index into the database keys by using the key prefix system, but overall they have no way of providing true support for columns as a relational database would
- Can work around this limitation by saying that each application has at least one column family, all keys are prefixed with date-time, and then the category of information is going to determine the structure of the key and the value. Consequently, the column family is going to specify the source.
- Example Categories:
    - Financial Transactions
        - key = time,name : value = json {price, anything else}
    - Location
        - key = time : value = json {location, anything else}
    - Contact
        - key = time,entity name : value = json {contact info, anything else}
    - Calendar
        - key = time,name : value = json {event info, anything else}
    - Message
        - key = time,type,sender,name : value = json {message, message info, anything else}
    - Browser History
        - key = time : value = json
    - Usage History
        - key = start time, name : value = json {end time}

- Idea is that my abstraction provides a uniform and a safe place to store a multitude of largely unrelated data. If people want the data arranged in a different manner, or configured differently so that it is easier to query, then they can read it out and reformat the data
- This system depends in large part on the keyed seek of RockDB. I need to learn more about this.
- Key-Prefix Iterators
    - https://github.com/facebook/rocksdb/wiki/Prefix-Seek 
    - Seems like you can create a bloom filter for a specific part of a key and then filter out everything that is not included into this bloom filter
    - This post makes it seem like such a system is possible: https://stackoverflow.com/questions/73670453/rocksdb-range-query-on-numbers

Definitions:
- Group : RocksDB column family. All groups have a category, an app, and a name. Stores info from the specified app in this category in the group with this name. Have a one to one correspondence with column families
- Client: producer or consumer interfacing with the info harmonizer
- Category: specify the type of data stored, which will specify the format of the return information

info harmonizer is going to run as a service. It is going to use interprocess communication systems such as pipes, shared memory, and sockets to communicate with web extensions, python apps, and C++ apps. It needs to be a system service so that it can manage concurrent reads and writes as well as securely manage permissions for various plugins. We don't want arbitrary plugins just being able to read and manipulate this precious treasure trove of personal user information.

Default Groups:
- Plugin Column Family
    - key = name : value = {password, list of column families (groups) accessible by plugin}
- Category Column Family
    - key = category : value = {list of all column families (groups) that belong to this category}

TODO:
- Develop a C++ service on windows which uses RocksDB to store information and has a socket based interface for other processes to interface with it.
- Consider changing the name to data buddy. Your own personal data manager.

- ChatGPT recommends that I use boost.asio for my C++ server, and I have used Boost in the past and though it is large it has very nice functions, so I think it is an acceptable choice for this project.

5/10/23
- Boost.Asio Install
    - wget https://boostorg.jfrog.io/artifactory/main/release/1.82.0/source/boost_1_82_0.tar.gz
    - tar -xzvf boost_1_82_0.tar.gz
    - cd boost*
    - `./bootstrap.sh --prefix=/usr/local`
    - `./b2`
    - `./b2 install`
- To Link Boost.Asio
    - `-L/usr/local/lib -lboost_system`
- RocksDB Install
    - `apt-get install -y libgflags-dev libsnappy-dev zlib1g-dev libbz2-dev liblz4-dev libzstd-dev`
    - git clone https://github.com/facebook/rocksdb.git
    - `cd rocksdb`
    - `make shared_lib`
    - Only include files that are found in the include/ directory
    - This takes several hours to install
    - Can Compile Examples with:
        - `make static_lib`
        - `cd examples/; make all`
    - The far easier and better way to do this
        - `apt-get install librocksdb-dev`
    - To find the .so file location and include directory location run:
        - dpkg -L librocksdb-dev | grep cmake
- To Link RocksDB
    - Make sure librocksDB is in the library path: `export LD_LIBRARY_PATH=/data-buddy/rocksdb:$LD_LIBRARY_PATH`
    - Compile with the following flags: `-I ../../rocksdb/include -std=c++17 -L/data-buddy/rocksdb -lrocksdb`
TODO:
- [ ] Implement the server with the basic interface defined above
- [ ] Write a python library for interfacing with the server
- [ ] Implement the RocksDB logic for the server

- Boost Server Notes:
    - shared_from_this() allows for shared_ptrs to the current class to be made (https://www.boost.org/doc/libs/1_62_0/libs/smart_ptr/enable_shared_from_this.html)

- I have decided to not use Boost.asio just because it is too low level. 
- ChatGPT and this reddit post both recommend Web Toolkit. It seems modern and up to date and maintained unlike CppCMS.
    - https://www.reddit.com/r/cpp/comments/4945z3/what_is_the_best_c_webserver_framework/
- I'm thinking I may want to use an RPC model since this is far closer to what I am actually doing than actually HTTP web development

Installing Web Toolkit
- `apt-get install gcc g++ libboost-all-dev cmake`
- `apt-get install doxygen libgraphicsmagick3 libssl-dev libpq-dev libssl-dev libfcgi-dev`
- `wget -c https://github.com/emweb/wt/archive/4.9.2.tar.gz`
- `tar -xzv 4.9.2.tar.gz`
- `cd wt-4.9.2`
- `mkdir build; cd build`
- `cmake ..`
- `make`
- `make -C examples`
- `make install`
- `ldconfig`

Building with Web Toolkit:
- `g++ -o wt_test -lwt -lwthttp wt_test.cpp`

5/11/23
Shopping Scraper:
- Will scrape Target and Amazon first

- web toolkit seems like it is optimized for developing web appliations. This is not necessarily what I want. I just want a very simple http server. Oat++ seems like they might be a good option, so I am going to try them out.

- The files in the service do not follow my traditional snake case naming convention, however, I am going to leave them as they are for now since I want to maintain the connection between my service and the oatpp framework that I am using.

## Dpkg v. Apt
- Dpkg and Apt are both package managers
- dpkg works directly with .deb pacakges and doesn't handle dependency resolution automatically
- apt sits on top of dpkg and provides a more user friendly automated package management experience and resolves dependencies automatically

5/15/23
- Compiling the service
    - `cd service`
    - `mkdir build`
    - `cmake ..`
    - `make all`
    - `./data-buddy-exe`

5/16/23
- Definitions:
- Group : RocksDB column family. All groups have a category, a client, and a name. Stores info from the specified app in this category in the group with this name. Have a one to one correspondence with column families
- Client: producer or consumer interfacing with the info harmonizer
- Category: specify the type of data stored, which will specify the format of the return information

Connection Pools are a way in which multiple sockets can be reused to enhance performance. I am not worried about such a system at this time however.

There doesn't seem to be a hard and fast rule as to whether you should put parameters in the URI v. in the body of the request. I am going to default to body of request, so if I decide to convert to RPC based system the conversion will be simpler.

5/18/23
- Installing Nlohmann Json
    - `apt-get install nlohmann-json3-dev`

- You can open a select set of column families only if you are opening in read only mode
- Otherwise you have to open all column families
- You need to destroy all column family descriptors before you close a database
    - https://github.com/EighteenZi/rocksdb_wiki/blob/master/Column-Families.md
- It seems the only way to retrieve a range of keys is to appropriately configure the iterator for prefix iteration and then iterate through all of them to find the ones that you are looking for
- They do not seem to have any range functions and do not iterate through the keys in any particular order. They are able to use a set of bloom filters to filter out keys that do not match your provided prefix, however, if they do match your provided prefix then you have to iterate over all of them due to lack of order
- Because you are only able to filter by prefix, you want the most generic prefixes to come first and then get more and more specific afterwards, or you want to order your prefixes so that when you perform a lookup based on a certain index you are more likely to filter based on various prefixes. Or you have to search for all possible values of a given prefix in order to find your specific intermediary value.
- May want your key_params have enums so that you can do this automatically
- If people want to do more in depth analysis they can read out all of the values and then put it into a SQL database and then process it there. My database is only going to support relatively naive lookup
- There is no way to iterate by a substring of a key. You can only iterate by the prefix of a key. It is a very very simple lookup system. Treat it as such. It may iterate though values lexicographically. ChatGPT said that it should not do this, but docs seem to indicate that it does do this.
- For this first implementation is best to assume that all keys are stored lexicographically and that the only way to query them is by the lexicographical ordering of their prefixes. This should be good enough for many functions.
- You can iterate through column families, but you cannot iterate across column families. I think you are better off not using column families and just using prefix iteration for everything and storing all keys into default. Then you can easily extract all data of a certain category
- Column families can also be useful for deleting a large set of keys, however, I do not plan on doing this often, so it is likely best that we avoid this.
- Since I am going to be keeping a list of clients as well as categories and I don't expect these lists to grow very large, I can by default order all keys by starting their prefix with category, client, and then providing an abstraction that allows people to get keys across different categories and clients, by executing multiple queries
- DO Not have keys based on clients. Just include the client name into all of the values of every single key
- Keys will be constructed as Category::key_params and you will maintain a list of all the categories that you know

TODO:
- remove all code dealing with groups
- create insert helper functions to insert according to category and that make sure the client is included in value
- The main value proposition is going to be in all of the extermities that you implement for this server and how easy it is for additional plugins to start saving off user data, and the plugins that are able to take this data and extract cool insights from it. At this point I wouldn't be too focused on making this backbone server super flexible and easy to query. You just want it to be lightweight, easy to interface with, simple, and fast.

## 5/22/23
- When it comes to security remember that you don't necesarilly need all of these super complex and expensive cybersecure applications. Having a website that emphasizes transparency and security and a rating system like Visual Studio Code extensions can go along way in building trust and is often times more helpful than all of the complex engineering solutions.
- When performing range searches for numerical values you can only search via the not including the least significant digits. e.g. The range 1204 - 1406 could not be represented. We can find ranges 1000 - 1999, 1200 - 1299, and 1200 - 1209 since for each one we specify an additional digit which is less significant than the prior to narrow down our range. Searching for a different range will be much more difficult.
- The key_param and value_param types we should accept are int, float, string, and date
    - int, float, and string can all follow the C++ standards in terms of precison
- Certain clients will only be allowed to access certain categories
- To add a client to a category you must have permission to that category
- The viewer is also going to be a client, however, it is going to be standard protocol that everyone gives the viewer access to their category once the client creates it

## 5/24/23
- App Database Categories and Prefixes
    - client:: (name) = json with info about when issued and password
    - auth_token:: (token_num) = json with info about when issued and client issued to
    - category:: (name) = json with format of key and format of value
- Note: just make the key value store force the values stored to always be json objects and then optimize for this paradigm
- Note: the category, auth_token, and client category types should not be added to the app database. This is going to confuse the logic for the do_list_categories function. The conventions for these also do not need to be made available to external users.
- Will not be useful for decoding, since we do not want to perform two gets for every get issued by the user, however, it can be useful for providing the user information about the structure of the database as well as checking that inserts are formed properly.
- Create a class that wraps the RocksDB database. This class can make the assumptions that the RocksDB database keys are all jsons and can also maintain an internal set data structure which tracks categories if we do not want to have to go to the database for every lookup, however, at first we should just do this
- Tomorrow you MUST finish the server. You are signficantly closer than you think.

## 5/25/23
- Instead of making auth_token a property of the value that is stored for a client name, I am going to create a completely seperarate key prefixed with auth and store all client auth information in the value accessed by this key. This will make range delections for these authentication tokens far easier.
- As clients get added to a category I am going to create a new entry with their name in the value dictionary and will use the existence of their name as a way to check if they have authorization?
- oatpp has a way of converting a DTO type to a json object very easily, however, the DTO type must be defined in terms of maps and lists, which is all I really need, however, it must be defined beforehand, and I want my category json values to be able to be constructed on the fly
- They have to be built beforehand however, because they are using macros for the majority of their interface
- I could create a serializer and deserializer for a base class type which has an adjustable structure based on the definition and then use this serializer with the framework, but I think that this is far too much work
- I don't want to constrain the value that I store to RocksDB to be just dictionaries, however, because the data transfer interface is limited my database may need to be limited
- However, I can remove the dictionary constraint on the RocksDB and then just have the values sent by the client be strings that are registered in json format and then just require that the category defined is able to validly check if the json string which was sent over is of type category and can be validly stored
- I'll need to build helper functions on the server side to deal with the strings, howeveer, this would make the storage far more versatile, which would be convenient.
- the rocksdb wrapper will consequently store strings and must be given an encoding and it will check that the provided string meets the encoding specified
- I believe that I can use the json schema validator to satisfy my requirements above

## 5/26/23
- A total ordering exists in the rocksdb which maintains the order in which keys were inserted into the database
- Normally, this total ordering is not maintained if the prefix extractor is used, however, some readOptions can be set to allow you to do this.
- https://github.com/EighteenZi/rocksdb_wiki/blob/master/Prefix-Seek-API-Changes.md

## 6/2/23
- It turns out that the json schema validator I thought was included in nlohmann's library is not actually included. I therefore, need to install the latest version of nlohmann's json library from scratch, as well as this json schema validator library. They both have cmake files that seem to be configured for the cmake install location. I am consequently just going to try to pull them down and install them into the cmake directory.
- Installing nlohmann json library
    - wget https://github.com/nlohmann/json/archive/refs/tags/v3.11.2.tar.gz
    - tar -xzvf <download>
    - cd <directory>
    - mkdir build
    - cmake ..
    - make install
- Installing json schema validator
    - wget https://github.com/pboettch/json-schema-validator/archive/refs/tags/2.2.0.tar.gz
    - tar -xzvf <download>
    - cd <directory>
    - mkdir build
    - cmake ..
    - make install
- The above steps should be the same for all cmake based libraries
- You will then have to link in the installed libraries to your existing cmake file
- Consider getting rid of the schemas for the puts and the gets. If a category requires a certain type of schema you should be able to validate a put or get schema based on the category of the value that they are putting or getting.

## 6/3/23
- I was considering returning the schema of the response in the response message, but I think this is overkill. All of my responses will be json objects and can be parsed as such. I will need online API endpoint documentation and should generate it by looking at the DTOs.hpp file, and should consider doing other code generation maneuvers for my endpoint, but not at this time. That documentation will come later.
- I think your github credentials may be stored in your container, which you have publicly hosted on DockerHub. I would recommend making sure that this is not the case.
- It turns out that an oatpp String is actually a shared_ptr to a string. I was under the impression that it was a custom implementation of std::string and could just be casted directly. Unfortunately a shared_ptr cannot be casted to a std::string, but I believe a type of std::string can be automatically casted to a shared_ptr. Consequently, going forward the rule is going to be that we are going to use std::string for all of the internal facing stuff, and are only going to use String in the Controller.hpp, since that is a type that oatpp will recognize.

## 6/6/23
- The json schema seems to have to follow this standard: https://json-schema.org/learn/getting-started-step-by-step
- The _json is an operator that allows you to pass it a string literal and it will convert it into a json object. Use this when you wish to just hard code strings into json objects.
- It makes sense to me that the RocksWrapper should ensure that only json objects are going into the database and then only json objects are coming out of the database. We are consequently going to use these json objects throughout the internals of the server and are not going to bother passing all of these objects around as strings
- If you need to optimize for space, since json objects do take up space, because you are recording the structure of every key that is going into the database you can potentially define your own compression scheme that is able to store a csv or something of the json, then read it out, put it into the appropriate string literal, have it parsed by the json library and returned as a json object
- Additionally, all error checking for the conversion from string to json and the well formattedness of values is going to take place in Controller.cpp. Any checks that are specific to the endpoint being called will take place here.
- Validation of all parameters regarding the structure of the json keys and values will take place in RocksWrapper.hpp
- We will still return errors as strings by convention

## 6/7/23
- It isn't the cleanest thing, but any response I have that is a json object is going to be returned as a string. The user will then have to parse this string as a json object if they wish to decode it. All values returned from the server will be strings as a consequence.
- In order to store the AES encryption value in a json object I am going to have to encode the auth_token in a base64 format. Otherwise I may be trying to store special characters in a json object, which it isn't a fan of. The Base64 encoding (https://en.wikipedia.org/wiki/Base64) ensure that only alphanumerical values are used when encoding.

## 6/8/23
- I am adding the additional constraint that all required keys must be the first keys stored in the json and must be stored in order
- The ordering of subsequent keys is unimportant
- This has important implications for performing range based lookups in the database
- The C++ chrono library is the library to use for all of your date time needs: https://cplusplus.com/reference/chrono/
- The order within json objects continues to be a problem. Consequently, I think that I am going to have to manually construct keys from the passed in json objects and will construct them in the order specified in one of the properties in the schema. This will allow me to validate the various components of the key, while still maintaining the ordering constraints.
- Actually, I can just use the required array to validate the ordering when constructing a key, since the values in the ordering are preserved
- Any properties that are left blank will appear as blank when querying and will likely affect the query, but this is ok, since that is a user problem
- I'll have to required that properties that come prior to other properties cannot be empty
- I'll also have to require that the category property is among the categories that are in the database
- This is also a good defense against "json injection" attacks if you will, since it is harder for people to manipulate the key that is being queried and the category property is guaranteed to exist so we can check if a user is authorized to access a given category property.

## 6/9/23
- I am going to create an abstraction for making requests to the server that you are sending json objects, so that in python you pass in a json object, but we convert it to a string during transfer, and then when it is transferred and decoded we decode it as a json object
- It may be good to add some trusted client system, where if a client is known and marked as trusted by the user we can bypass formatting checks. Won't worry about it until writing becomes too difficult because of the checks however
- I believe that I have managed to wrap up the implementation of this service. Below are the things that I need to do to touch it up:
    - Remove the db-test endpoint and the db-get-test endpoint
    - Review and rewrite code
    - Add error checking for all required parameters in endpoint functions in Controller.hpp
    - Figure out how to get the AES key string from the environment instead of just reading it from the file
    - Figure out how to store values passwords not in plaintext
    - Figure out a better way for encrypting the rocksdb database, so that it is difficult to extract data from it
    - Figure out managing the json strings
    - Potentially switch to a different server framework
    - Track resource utilization and figure out how threading works for this thing
    - Uses tasks.json to make it easier to build your file. Also have a more consistent way of managing json, python, and c++ when it comes to building. Need some kind of package manager or place where library files are kept. Don't want to be copying APIs all over the place.
    - You will want your server to suppport https or some sort of client to server encryption that way you are safe from MITM attacks
    - In ther server parameters and structs and classes should have members stored in decreasing order
- At this time however, the MVP has been implemented. I will come back and do these fixes at a later time or as necessary. In the meanwhile I am going to focus on the next phase of the project which are the information retrieval programs.

## 6/13/23
- I believe that scraping data from Amazon will be the most beneficial to focus on since a high volume of items purchased by individuals are bought on Amazon, so being able to scrape information off of their could be very beneficial to people.
- Amazon Data Scraping options:
    - Request CSV of Order History:
        - Go Here: https://www.amazon.com/gp/privacycentral/dsar/preview.html
        - Select Your Orders and Hit Request
        - Click in the confirmation email
        - Wait up to 1 MONTH for data to be sent to you
    - Scrape website from viewer perspective
        - Seems to be a way in which one can use a plugin to inject code into a website and then simulate a click: https://stackoverflow.com/questions/11387615/how-can-i-make-a-chrome-extension-automatically-click-a-button-when-a-page-loads
        - Could rapidly click through all orders to extract order information
        - Would be nicer if we could do this for continual updates
        - Could then just ask the user to navigate to the page and we can take it away
        - Would need to remind the user that they need to update their information or something along these lines and make it incredibly easy for the user to navigate to these pages
        - Can avoid saving passwords and just have the user navigate to the pages and perform logins
- Future sites to extract data from:
    - Costco
    - Target
    - Aldi?
    - Jewel Osco
    - Comerica
    - Vanguard
    - Coinbase
    - Intuit Mint
- Goal: create an inflation calculator that will compute how the price of similar goods has changed for you month to month as well as tells you amount spent on groceries, etc.


## 6/14/23
- Objectives for today are:
    - [ ] Get a popup page to appear when you click the extension
    - [ ] Have a button in the popup that when you click the button it runs a script
    - [ ] Have the script click a button on the amazon orders page, that will take you to see more details about an order
    - [ ] Once you reach the order page just have a script automatically run, which switches back to the previous page
- Extension Notes:
    - Files specified in the background cause the main DOM to be executed in the background
    - If you include an html file in the action parameter the html you specified will be rendered in a window that is at least 25x25 and cannot be larger than 800x600.
    - I don't think that I want the background attribute right now. I think I want the popup attribute right now, and will then just click the button to start the data extraction
    - It seems that the action API is the primary API for interfacing with the extension that appears on the bar
    - Outside of this action API there are ways that the extension is able to interface with the browser
    - In order for things to log to the console you have to generate an html popup and then select inspect on that popup NOT ON THE PAGE
    - The action API can also be used to display the popup
    - Web_accessible_resources allows me to specify which files from the root directory are part of the project, so that they can be imported and referenced for extension use
    - I hate manifest.json. It is complete bullshit and doesn't follow any coding paradigms
    - Consequently, I am going to set the whole thing up as a background.js file and have this be the main file an then use the action API to make all of my calls into things
    - By convention I am going to start all paths with ./ so that I can be consistent and easily find files relative to the root directory
    - It seems that many things have changed in V3 from V2, which is a bit of a pain, since there are less docs on it, however, I'll live
    - In order to allow calls to external sites you need to set the host_permissions in the manifest.json file which will allow you to make requests to the server that you are running locally
6/15/23
- Goals for today:
    - [ ] Look into Visual Studio code experimental and see if you can enable github copilot chat
    - [ ] Research existing Amazon order web scraping plugins and maybe use one of these instead
    - [ ] Consider automating the web scraping process. Can it be done through ML? Can we develop some sort of trainable user interactive one where the user does one cell and then it does the rest and keeps doing it every time the site is visited? Could also have the existing plugin highlight things in the DOM that it extracts so that people know exactly what it is extarcting and can monitor its progress.
    - [ ] Debug why the get-list-clients command is crashing the data buddy server
- I managed to download all of my personal data from Amazon. It was a bit of a pain, but the downloaded data file does have all of the information I want formatted how I want it, so this is something to consider. If your product is targeting more technical users this may be a good first step. You could just have some sort of guide that allows them to download their Amazon order history. I think the added value of your product however will be its ability to extract information as people visit and use sites. This may be a great thing for new users to do for setup, but then afterwards it would be nice if the plugin was able to register new purchases or something along these lines.
- If you wanted to have some sort of data validation you would need to talk with the companies that are providing the sites generating the data and have them verify that your data was successfully verified
- Alternative Amazon Web Scrapers
    - https://www.scrapingbee.com/scraperapi-alternative/?utm_source=google&utm_medium=cpc&utm_campaign=11870883009&utm_term=scraper%20api&gclid=Cj0KCQjw7aqkBhDPARIsAKGa0oISFWtybLoUyVWohujW9YHObqMgJUz75beTBvP464lu-pvn38cqCmEaAvRUEALw_wcB
        - Doesn't seem very powerful or able to be tailored
        - Just seems like this API that you give it a url and it returns you the fully rendered web page
        - I can have my extension get to the web page. I need a way of processing the info on the web page
    - https://www.asinspotlight.com/?utm_source=google&utm_medium=cpc&utm_campaign=19343361988&utm_content=141975517622&utm_term=amazon%20data%20scraper&gclid=Cj0KCQjw7aqkBhDPARIsAKGa0oIJjLQbnu8hTyxYK6t5hNLXcRPHJiEpAlt528NIGpYiqIUixgoFhjwaApfREALw_wcB#section-research
        - Above is another alternative however they as well don't have any support for scraping user data, it is just general Amazon site data and pages that do not require login for scraping
    - This appears to do exactly what I want: https://github.com/philipmulcahy/azad
    
- After inspecting the code associated with the Amazon web page I don't think it will be too difficult to manually scrape it. The more challenging part is going to be navigating to various pages to extract information. Also, are you going to have to query the database, so that you can validate what you have acquired and what you haven't? May not be able to make this thing write only.
- Server is crashing after attempting to access a database even though it hasn't been initialized. I need to add a check that makes sure the database is initialized before continuing.
- I also need to see if there is a way to have a subsection of an external git repository in my repository. I don't want the whole thing, just a subset.

## 6/16/23
- The current plan is to add a button to azad, which will automatically write the found data to data buddy
- Once the button is added we are going to develop a consumer which we can then use to play with the data that we gathered
- chrome.runtime api is used for message passing between extensions and different scripts running within the extensions
- I will want to create a .git/info/sparse-checkout file and add it to my amazon web scraper. This sparse-checkout will extract files from the azad github repository. I will use their processing files, but then build a new UI and interaction system on top of his processing files. When he rolls out updates I want to have a script that is just going to be able to pull the updates and the sparse-checkout will surely enable me to do this

## 6/17/23
- background.ts interfaces with the data extraction scripts, so I certainly want to avoid using background.ts and just communicate with these scripts directly
- inject.ts seems to be the file that I am going to want to interface with
- azad_order.getOrdersByRange may be the function that I would like to use and define for my interface
- I think order.ts and item.ts are the two important files that are doing all of the extraction
- git worktree add and git sparse checkout seem like a pain in the ass. I am just going to write a bash script, which clones the whole thing, copies out the files that I want, and then deletes the whole thing
- azad seems really slow. Rewriting in rust may be a move

## 6/18/23
- Am going to develop the web extension in typescript since that is best practice. Am setting up the extension folder using these guidelines: https://betterprogramming.pub/creating-chrome-extensions-with-typescript-914873467b65
- webpack bundles and compresses js and ts files for production use

## 6/19/23
- The structure that I have for the browser_plugins should be consistent across projects. I want every folder in the ts folder to be reduced to a single file and then included by manifest.json.
- All other files in the src directory should get copied
- All the setup and testing scripts should be outside of the src directory
- All output files are going to be written to the dist directory
- You should not compile the code in a docker container, but should do this web development stuff on the local system. Compilation is significantly faster on the local system and containerization isn't that important since all installs are well managed by npm and the application itself is running inside a container, which is the browser, so you shouldn't be making too many edits to the filesystem.
- The purpose of the service worker is to run in the background, manage network requests, deal with caching resources, and not interface with the DOM of the webpage (Web assembly is a good candidate for replacing this file)
- Context scripts have access to the DOM and are meant to interface directly with the webpage. They can then use chrome.Runtime ports to communicate with the service worker so as to have it handle network requests and the like. They are injected into specific web pages for processing. You should have a file that manages the popup 
- Should also have a popup.js containing all of the js for the popup that is created
- These should be the only 3 files that are generated in the dist directory: control.js, inject.js, background.js and these should be the only 3 folders in the ts directory in src
- Add definnitions defining the purpose of each of these files
- Adding a question mark at the end of a variable name in typescript indicates that if it is null, then don't interact with the thing and thrown an error
- Adding an exlcamation point at the end of a variable name in typescript is an assert that guarantees an element is not null and will thrown an error if otherwise
- Going to use project references and restructure accordingly so that typescript files in background, popup, and inject are treated separately
    - https://www.typescriptlang.org/docs/handbook/project-references.html
- In order for each of background, inject, and popup to be compiled and checked separately I had to create a tsconfig.json and a webpack.config.x.js for each. Now simply running npm run build will build all 3 separate projects. To save time you can elect to recompile just one of the projects pretty easily.
- Webpack seems to require an entry point. I was unsuccessful in getting it compile without one
- It is best practice in javascript to use the let keyword when declaring variables since the var keyword is not properly scoped
- For the sake of code organization I have been trying hard to properly configure my tsconfig files. However, I now realize after viewing this issue that I can configure the project in a modular way using the tsc loader, however, webpack tries to use tsc loader under the hood, but is fucking it up and I cannot properly configure my webpack.config.js file.
    - https://github.com/TypeStrong/ts-loader/issues/647
- Consequently, I am just going to use one tsconfig file and an going to build all 3 separate modules together using one tsconfig. I know that this will work, however, it will also lead the program that think that they are all part of one library, which is not true, however, it won't have a huge effect since webpack is going to split them apart into separate libraries. I just need tsc to compile all of the code together and then webpack will go through and break things apart.
- Webpack bundles and tsc transpiles. You can configure tsc in a way that it transpiles 3 separate modules that all depend on one another, but when webpack uses tsc for transiplation you cannot configure tsc in this way.
- I have been getting overwhelmed with how daunting a task making this extension seems like it will be. I am going to break it down into smaller more manageable pieces, which will help me better quantify progress as well as motivate me to continue working on the project.
- Plan
    - [ ] Setup the repo
    - [ ] Develop a library that will abstract away the API for interfacing with the data buddy service
    - [ ] Develop an interface for the background.ts that the inject.ts will be able to communicate with and that can speak with the server
    - [ ] Develop the code that is going to leverage the web scraping files from azad and will then extract the scraped information and send it to the background, so that the background can write it to disk
    - [ ] Develop the popup so that it will show you the progress of the data extraction. Should only have a progress bar and a go button. It should also show the location of the data buddy folder being written to.
- There is a way to make storage persist across instances of the extension. I should use the across device storage for storing the extension credentials. I should use local storage for storing the database path.

## 6/20/23
- In my extension I am not going to give people the option to create a data buddy instance. Instead they are only going to be able to connect to one or disconnect from one. Creating the instance will be reserved for the service manager that I will develop later on. For dev purposes, however, I may add a create button
- May want to add some sort of ping function for the extension to check the status of the local service.
- There are several states that a popup can be in that must be encoded
    - Waiting to connect
    - Connected not scraping
    - Connected and scraping
- The background will control which state the popup is in. The state that the popup is in will determine what messages it is able to send to the background
- I had to make the API abstract away the fact that oatpp errors are returned as strings while data buddy errors are returned as json objects. The service_api_lib has alot of code for handling this unfortunately. Everything should be able to be parsed as text. I may want to just return text or I can be chill with returning the json.
- You are going to need to establish some sort of ping function on the server, so that the client can tell that it is still connected? For now, don't worry about that and just say it is connected until you get some sort of error message and then move to disconnected. Configure this in the background.ts file.
- Do you know what happens if you send a create message to a directory that already has something? Does it delete everything or make something new?

## 6/30/23
- I came to the realization that if I am going to break apart my extension into the various components that I laid out in my C4 diagram that I can have my background component be written in rust, since it is not directly interfacing with the DOM. My inject.js and popup.js are interfacing with the DOM, so I will want those to remain in javascript, but this is a good opportunity for me to use Rust for the background service, which it is probably better that it is in rust, since I'll need it for caching information and performing fast queries to my C++ service.
- Wasm bindgen is used to facilitate javascript to wasm interactions, so I think that the entrypoint for the background.js is going to be javascript, but the actual application will be running wasm
- While I'm modifying my extension setup I may as well just convert from using raw html to using react or vue.js
- I am going to go with react, since it is a library and not a framework and because I don't have much experience with it, so it will be a good learning tool
- Rust Notes:
    - mod declares a module
    - pub makes functions within a module accessible from outside that module
    - fn declares a function
- Comlink is a libary that allows for javascript code to communicate with rust code
- Webpack is configured in such a way in this repo to compile everything to dist and then copy everything that is in public into dist, and then execute dist
- There seems to be some sort of way of linking the wasm worker with the react app when declaring the app itself
- I take the delcaration of the app to be a very well defined thing, and consequnetly I blieve that there is some sort of special protocol for connecting the webworker to the application via App component in the popup.tsx
- Webpack seems to just know how to compile jsx, react, and typescript files, so as long as you specify a file as jsx or tsx webpack is going to know how to compile it and will return the appropriate javascript
- Wasm-pack is another tol that builds web assembly from rust
- Compile rust is put into the pkg folder in the webnav_analysis folder
- It is then read as a dependency from the package.json file, which is then compiled into the dist folder
- Finally the application is loaded as an extension from the dist folder
- React
    - React components are javascript functions that return markdown (html)
    - You can have markdown that has capital letters and we know that these refer to react components and not actual markdown
    - JSX is a special type of markdown. It is to html with typescript is to javascript
    - use `className` instead of `class`
    - curly braces let you exacpe back into javascript
    - can pass javascript functions to certain html attributes such as onClick
    - react's useState can be used to generate state which persists throughout the life of the application
    - App is the main entrypoint. You should have an app component inherited from react in your index.js and from that it will call into your custom javascript function
- WasmWorker
    - Just an interface for the comlink api
- comlink and web workers
    - RPC implementation for postMessage, which is what we were using in javascript to communicate from the application running in the DOM to the application running in the background. The same system of communication must be used here.
- Why is background.ts just empty? Does it need to be there for compilation purposes or is it just unnecessary?
- How is the wasm module loaded in the actual application
- Wasmworker is where the webnav_analysis module is imported and this module is develoepd with rust
- functions from this rust module are then used in and returned in the javascript code

## 7/1/2023
- It is July. I got bogged down in making the Amazon history extractor extension. One lesson that I have learned is that if you get bogged down solving one problem, go solve a different problem, get some momentum, then come back to solving the original problem.
- One of the primary reasons that I started developing this application was so that I could get exposure to the new big data and machine learning wave, as well as utilize it as a way to enhance my probability knowledge. This July I am going to focus on building a consumer which will be focused around one of these areas. I would like to use random processes, markov chains, or machine learning techniques for this consumer. Whatever data I need I will manually collect and will just pump it into my database and then build the consumer to read the information out from the database.
- Ideas:
    - Duration analysis
        - Time spent commuting
        - Time spent on any webpage
        - Time spent on computer
        - Time spent in a particular location
    - AI Chatbot
        - analyzer that tells you how often you wear particular articles of clothing and what your most and least worn clothes are and recommends what clothes you should donate
        - ebay assistant that looks at buying and selling trends on ebay and gives you advice on how to price various items
    - Failure analysis
        - probability of car or boat breaking down
    - Finance analysis
        - expected amount of money spent while in a particular area (multiple random variable)
        - expected amount of money spent to get an Uber
        - expected amount of money spent on rent in an area
        - Expected amount of money donated each year
        - expected amount of money spent on credit card
    - Amazon analysis
        - Expected value paid on amazon order
    - Email analysis
        - A company called SayMine is able to analyze which companies use your data just by looking at your email
    - Calendar and contacts analysis
        - who do I hang out with the most
        - who haven't I reached out to in a while
Based on this medium article: [Title](https://towardsdatascience.com/what-is-the-value-of-your-data-9341cd019b4d) it doesn't seem like our data is worth very much. However, the most expensive data points it seems are as follows:
1. Health Data (age, gender, personality traits, physical and mental health)
2. Demographic Data (age, gender, political affiliation, religion, sexual orientation)
3. Identity Data (name, age, gender, profession, income, address, picture)
    - Income analysis
        - suggestions for how you can increase maybe from tax perspective
    - Taxation analysis
- Chat bots advantages:
    - an intuitive way of querying for information
- Could just make a chatbot that learns the structure of the database and based on this structure allows you to ask it various questions about your personal data
- Github copilot right now takes your current file as well as current open files, puts them into the ChatGPT query and then executes the query using these inputs. It doesn't always know everything about your code base all at once
- Retriever is the technical term for the application which is in charge of supplying all relevant information for a particular data query
- Retrievers are trained alongside ML models so that they can learn what the most relevant data is that they should be retrieving
- Convert all database info into a vector database and we then vectorize our quesiton and we compare the vectorized question with the data in the database, so that we can get the most relevant pieces of information
- Reader model then takes the retrieved context vectors and reads the information within each of these context vectors and extracts very specific information from a larger chunk of data
- Long tail in Q&A questions, so depends on use case, since if there is some very niche data and there isn't alot of online data on it it will be more difficult to train and you will have to train yourself, but if it is general can just used a pretrained model

## 7/3/2023
- My current plan is that I want to develop a retreiver and try to plugin the retrieved information into the LLM and get interesting results. I would need to configure the LLM to be receptive to the data format that I retrieve, or I could reconstruct the information retrieved in a way that is more conducive and helpful to the LLM. The data that I am going to start with is Google timeline data. I will see what I am capable of deducing from this data. I am going to first put this data into data buddy and will then read out the data from data buddy so that I can ask the LLM questions regarding it.
- The basic idea is that you want to engineer a data rich and complex query that takes in a lot of relevant input and then have ChatGPT give you the response. If you want ChatGPT to take what you are giving it as input you need to convert your input from a basic data key, value to an actual query. The main engineering is figuring out what data to provide it and the best format to provide it this data. ChatGPT has limited input, so we want to be smart about the size of the text prompts we make.
- ChatGPT can be thought of as a machine learning model which has learned to understand and interpret human language. The interface of human language is incredibly imprecise which gives the model incredible flexibility, but also limited in accuracy and tunability.
- It seems that Facebook is actually using this technique in their SOTA chatbot that has the great added value of being able to remember things for a long time: [Example](https://ai.facebook.com/blog/blender-bot-2-an-open-source-chatbot-that-builds-long-term-memory-and-searches-the-internet/). At the end of the day however, all they are doing is adding an extra step to ChatGPT where you have a system for searching for information that is relevant to the question being asked at hand, condensely provides this information to the text processing model and then returns this result.
- I want to avoid training an AI if possible, since it is expensive and time consuming and more of an art than a science. The data that I am collecting can be retrieved, given a good deal of structure, and then named entity recognition or some other model could be used to get the most relevant information and then return this to the user. You don't necessarily have to use some special text embedding system for retrieving this information, you could quite naively just do some categorical date search. You can even incorporate prompts into the Chatbot that allow it to ask the user for multpile rounds of information which will help it to better search through its data store and report the relevant info to the user.
- You can connect Wolfram Alpha and ChatGPT and can then get the most relevant response based on the question.
- This architecture would also allow you to be able to develop a highly effective system, which doesn't require tons of random user's very personal data to train. You can do all of this using models that were trained to solve other tasks and just start gluing them together.
- This whole system is what the ChatGPT plugin system is baded on.
- This Chatbot is one example of a consumer of the data that I am collecting. Is there a better example that I could be using?
Assuming I have the below data what consumer applications would be cool to make?:
- Email
- Music
- Contacts
- Calendar
- Social Media
- Shopping
- Finance
- Passwords
- Health
- Technology usage
- Search
- Github
- Slack
- Hotel sites
- Clothing sites
- Airline sites
- Movies
- Documents
- social media posts

Consumer Brainstorming ideas:
    - Give it the bible, encode text segments of the bible and have it interpret rules of the bible and how you can apply them to your life?
    - Can then combine biblical rules, as well as catechism rules, as well as your own personal information and have it give you helpful insights and ways that you can improve as an individual
    - Create a chatbot that people can interact with to query random information about theirselves and their personal lives. They can then leverage a combination of wolfram alpha and chatgpt to get mathematical as well as creative answers.
    - App that more intelligently recommends Christian alternatives to particular music?
    1. Metric Tracking Applications
        - Automatic habit tracking
        - Health tracking
        - Spending and wealth tracking and net worth and finance
        - Clothing usage tracking
        Donation tracking
        - Media exposure analyzer and how much media you consume of different types and if it is of redeeming value or just garbage
    2. Recommendation Applications
        - Optimize the amount of stuff you own
        - Optimize how to be more secure
        - Optimize for money
        - Optimize for time (how to use your time better)
        - How to becomre more catholic
        - Optimize for health and eating healthy food
        - Optimize your life for multiple categories
        - Optimize the amount of stuff you own
        - Teaching application that teaches based on how you learn and teaches things that it believes will be useful to you based on your profession.
        - Career coach
        - Technology literacy teahing coach
        - 1% improvement app
        - Movie recommendation app
    3. Automation applications
        - Analyze who has your data and automatically request these companies to delete this data of yours
        - Attempt to detect patterns in your behavior and starts to execute these patterns automaticlaly without your involvemnet. Maybe like buying a recurring plane ticket or paying a credit card bill or something
    4. Enhanced Personal Applications
        - Email
        - Contacts
        - Calendar
    - Survey participation application. Application that asks data buddy to provide various pieces of info about a user that weren't normally collected until now and the user can elect to participate in the survey or not. Could be like research study groups and they can offer to pay you for your information instead of having to survey you in person.
- Note: It is difficult to develop good applications using personal data, when you don't have access to large databases of the data.
## 7/5/23
- These are all good ideas and potential directions. For starters I am going to keep on going with location information pumped into ChatGPT. It shouldn't take me long.
- Information retrieval plan
    - Have embeddings built that are year, month, day, time, (year month day), (year month)
    - In order to do this the internet recommends I develop my own Named Entity Recognition tool using Spacy: [internet](https://datascience.stackexchange.com/questions/87981/extract-date-duration-from-text)
    - I've already done this for a school project and am not going to learn anything by doing it twice, so I'm not going to do this consumer project.
- Life suggestion application would require me to leverage ChatGPT as well as my own recommendations. I would need to analyze different data that is provided to data buddy and then make suggestions. We could start very specific by analyzing location data and tracking the amount of times that you go to Church and recommending that people go to Church more. Let's start with this.

## 7/8/23
- Finance Category Key Structure
    - Date
- Finance Category Value Structure
    - Name
    - Date
    - Amount
    - Debit/Credit
    - Account
    - Description
    - Category (From Mint Categories)
    - Sub category (From Mint Categories)
    - Source
- [Mint Categories](https://mint.intuit.com/mint-categories/)
    - Income
        - Paycheck
        - Investment
        - Returned Purchase
        - Bonus
        - Interest Income
        - Reimbursement
        - Rental Income
    - Miscellaneous
        - Cash & ATM
        - Check
    - Entertainment
        - Arts
        - Music
        - Movies & DVDs
        - Newspaper & Magazines
    - Education
        - Tuition
        - Student Loan
        - Books & Supplies
    - Shopping
        - Clothing
        - Books
        - Electronics & Software
        - Hobbies
        - Sporting Goods
    - Personal Care
        - Laundry
        - Hair
        - Spa & Massage
    - Health & Fitness
        - Dentist
        - Doctor
        - Eye Care
        - Pharmacy
        - Health Insurance
        - Gym
        - Sports
    - Kids
        - Activities
        - Allowance
        - Baby Supplies
        - Babysitter & Daycare
        - Child Support
        - Toys
    - Food & Dining
        - Groceries
        - Coffee Shops
        - Fast Food
        - Restaurants
        - Alcohol
    - Gifts & Donations
        - Gift
        - Charity
    - Investments   
        - Deposit   
        - Withdrawal    
        - Dividends & Cap Gains 
        - Buy   
        - Sell  
    - Bills & Utilities 
        - Television    
        - Home Phone    
        - Internet  
        - Mobile Phone  
        - Utilities
    - Auto & Transport
        - Gas & Fuel
        - Parking
        - Service & Auto Parts
        - Auto Payment
        - Auto Insurance
    - Travel
        - Air Travel
        - Hotel
        - Rental Car & Taxi
        - Vacation
    - Fees & Charges
        - Service Fee
        - Late Fee
        - Finance Charge
        - ATM Fee
        - Bank Fee
        - Commissions
    - Business Services
        - Advertising
        - Office Supplies
        - Printing
        - Shipping
        - Legal
    - Taxes
        - Federal Tax
        - State Tax
        - Local Tax
        - Sales Tax
        - Property Tax
- manager is going to be the name of the local electrum application that has access to the entire database and is able to show the user a multitude of information. You are going to have to establish some special rules on how to create it, so that another application cannot impersonate manager, since it should have access to every application
- Will want to have default values for where stuff is installed and where the command line utilities and such are able to locate various files for parsing purposes
- Tauri v. Electrum
    - I was comparing and contrasting Tauri v. Electrum for the manager. 
    - I want the manager to be able to run websites and plugins from other developers, and electrum has support for this functionality through a system called BrowserView.
    - Tauri does not have support for this feature, so I am going to stick with electrum
- I am going to follow the Rust naming convention going forward because I like it
    - Rust Naming Conventions: https://rust-lang.github.io/api-guidelines/naming.html#:~:text=Basic%20Rust%20naming%20conventions%20are,%22value%2Dlevel%22%20constructs 
	    - Types, Traits, and Enums (All Custom Object types) should be UpperCamelCase
	    - Everything else is snake cased (including file names)
        - Statics and contants are SCREAMING_SNAKE_CASE where everything is capitalized
- Configure the service such that the manager is the only one that connect and disconnect the service to different databases. All other applications will just be able to ping the database to see if it is alive and will be able to check if it is connected, and if it is connected they can start writing to it.
- For all of my python files I am going to opt to use single quotes when writing python files for consistency sake
- it seems that mint is being annoying and is not giving me a full list of their transaction types. For this reason I am going to use spacy to check which of the mint categories is most closely associated with the provided text. I may also be able to use spacy for categorizing things which we aren't sure about
- Installing and setting up Spacy
    - `pip install spacy`
    - `python3 -m spacy download <name of model. e.g. en_core_web_sm>`
- Possible good starting application for consumer side of stuff will be a category filter that is able to go through all finance data easily and allows user to merge together similar stuff and then overwrites those values back into the database
- Primary objective and first use case is going to be the consolidation of financial data and the ability to analyze it and get a nice holistic view of it
- Could create a data mixer where people can voluntarily put their data for free, so that new and more interesting machine learning can be done and the whole thing can be anonymous and relatively security friendly
- Data mixer would be free and some people may write false data, but that's fine. Will hopefully only be a small portion of people

## 7/9/23
- TODO:
    - [x] Manipulate the intuit mint data file that you have, so that the information is not real
    - [ ] Create a regression testing script for the file parser 
    - [ ] convert the finance directory into its own package that you can import into the main file and can then just very quickly check the type for all finance
    - [ ] create a parsing file for the amazon order history stuff
    - [ ] start working on the data buddy manager CL application and rethink the security system you have in place

## 7/10/23
- Something cool to do may be to make an application which tracks the total amount of money that you give to the government including taxes form all individual purchases

## 7/18/23
- Creating a module in python
    - Should always be structured where you have the outer pkg folder
    - inside you have a folder that is the name of the module and in this folder you have an __init__.py and a main.py file
    - outside this folder you have a requirements.txt which can be generated with `pip freeze` and then you have a setup.py file
    - The setup.py should simply contain what is in service/api_libs/service_api_lib_py_pkg/setup.py
    - If something is a module it can import stuff that is in a higher directory
    - pkg_resources can be specified in the __init__.py file to import external files that a package may be dependent on
    - Once your package is constructed you can install it with pip
- Testing
    - When testing python files pytest should be used
    - install pytest with `pip install -U pytest`
    - test files should have the prefix test_ followed by the file name
    - pytest will then run the test files
    - Ran into issues with installing the module. Can create the installable package with:
        - `python3 setup.py install/develop`
    - Then need to install the package with pip by going pip install -e <path to setup.py>

## 7/23/23
- I am going to leave the service_reg_tester.py file as it is. I don't feel like refactoring for pytest and since it is an integration test I'm not sure that pytest is going to be the best framework for making it work. It seems like pytest is more useful for unit testing. Going forward for all Python files I'll be sure to use pytest, however.
- I am shelving this project for now, since I am just too busy. I'll pick it back up in 2-3 months.

# Ongoing List of Things TODO
## Server
    - Remove the db-test endpoint and the db-get-test endpoint
    - Review and rewrite code
    - Add error checking for all required parameters in endpoint functions in Controller.hpp
    - Figure out how to get the AES key string from the environment instead of just reading it from the file
    - Figure out how to store values passwords not in plaintext
    - Figure out a better way for encrypting the rocksdb database, so that it is difficult to extract data from it
    - Figure out managing the json strings
    - Potentially switch to a different server framework
    - Track resource utilization and figure out how threading works for this thing
    - Uses tasks.json to make it easier to build your file. Also have a more consistent way of managing json, python, and c++ when it comes to building. Need some kind of package manager or place where library files are kept. Don't want to be copying APIs all over the place.
    - You will want your server to suppport https or some sort of client to server encryption that way you are safe from MITM attacks
    - In ther server parameters and structs and classes should have members stored in decreasing order
    - Use clang to lint your server side code
    - Think about the auth tokens more carefully and when they need to be invalidated
    - Move json schemas into the predefined_objects.json file
    - Add special support for allowing the manager to be able to be created. Not sure if you want this in the server logic or to just happen automatically at install?
    - Refactor to comply with the rust naming convention
    - Add clang to provide linting and clangd for helpful resolution insights. Make sure you are using C++20 for compilation
## Web Scraper
## File Parser
    - Add ability of parser to load custom parsing code and parse file at runtime
    - use the importlib.resources to load in the json file, so that way when you create a binary you don't need that as a side import
    
## Manager
    - Make a command line interface for testing purposes and for users that don't want to use an application
