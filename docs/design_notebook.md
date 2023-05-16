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

