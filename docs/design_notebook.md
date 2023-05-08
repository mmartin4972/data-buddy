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

## Security Features:
- plugins are only able to edit certain data segments
    - for starters probably best to say that anyone can write anything, but no one can delete

## Plan:
- Implement a very base level of the system
- Use it on the grocery application

