#pragma once
#include <iomanip>
#include "Communications.h"
#include <curl/curl.h> //.\vpkg install curl


class NodeStatusData {

//NODESTATUS 1{Peers} 2{LastBlock} 3{Pendings} 4{Delta} 5{headers} 6{version} 7{UTCTime} 8{MNsHash}
//           9{MNscount} 10{LasBlockHash} 11{BestHashDiff} 12{LastBlockTimeEnd} 13{LBMiner}
//           14{ChecksCount} 15{LastBlockPoW} 16{LastBlockDiff} 17{summary} 18{GVTs} 19{nosoCFG}
//           20{PSOHash}*/
    
public:  
    std::string NodeStatus = "Empty";             // 0- Saves NODESTATUS Message, just to have control
    unsigned int Peers{ 0 };                       // 1- Saves Number of Peers connected to this Node.
    unsigned int BlockNumber{ 0 };                 // 2- Saves Current Block Number
    unsigned int Pending{ 0 };                     // 3- Number of Pending Operations
    unsigned int Delta{ 0 };                       // 4- Number of Pending Operations
    std::string Headers = "Empty";             // Number of Pending Operations
    std::string Version = "Empty";        // Current Node Version Software
    unsigned int UTCTIme{ 0 };
    std::string MNsHash = "Empty";
    unsigned int MNsCount{ 0 };
    //int Updated {0};
    std::string LastBlockHash = "Empty";
    std::string BestHashDiff = "Empty";
    int LastBlockTimeEnd{ 0 };
    std::string LastBLockMiner = "Empty";
    std::string ChecksCount = "Empty";
    std::string LastBlockPoW = "Empty";
    std::string LastBlockDiff = "Empty";
    std::string Summary = "Empty";
    std::string GVTHash = "Empty";
    std::string NosoCFG = "Empty";
    std::string PSOHash = "Empty";

    //Getters
	std::string GetNodeStatus() { return NodeStatus; }
	unsigned int GetPeers() { return Peers; }
	unsigned int GetBlockNumber() { return BlockNumber; }
	unsigned int GetPending() { return Pending; }
	unsigned int GetDelta() { return Delta; }
	std::string GetHeaders() { return Headers; }
	std::string GetVersion() { return Version; }
	unsigned int GetUTCTIme() { return UTCTIme; }
	std::string GetMNsHash() { return MNsHash; }
	unsigned int GetMNsCount() { return MNsCount; }
	std::string GetLastBlockHash() { return LastBlockHash; }
	std::string GetBestHashDiff() { return BestHashDiff; }
	int GetLastBlockTimeEnd() { return LastBlockTimeEnd; }
	std::string GetLastBLockMiner() { return LastBLockMiner; }
	std::string GetChecksCount() { return ChecksCount; }
	std::string GetLastBlockPoW() { return LastBlockPoW; }
	std::string GetLastBlockDiff() { return LastBlockDiff; }
	std::string GetSummary() { return Summary; }
	std::string GetGVTHash() { return GVTHash; }
	std::string GetNosoCFG() { return NosoCFG; }
	std::string GetPSOHash() { return PSOHash; }
	//Setters
	
    void SetNodeStatus(std::string NodeStatus) { this->NodeStatus = NodeStatus; }
	void SetPeers(unsigned int Peers) { this->Peers = Peers; }
	void SetBlockNumber(unsigned int BlockNumber) { this->BlockNumber = BlockNumber; }
	void SetPending(unsigned int Pending) { this->Pending = Pending; }
	void SetDelta(unsigned int Delta) { this->Delta = Delta; }
	void SetHeaders(std::string Headers) { this->Headers = Headers; }
	void SetVersion(std::string Version) { this->Version = Version; }
	void SetUTCTIme(unsigned int UTCTIme) { this->UTCTIme = UTCTIme; }
	void SetMNsHash(std::string MNsHash) { this->MNsHash = MNsHash;}

    //Getters
    void SetMNsCount(unsigned int MNsCount) { this->MNsCount = MNsCount; }  
    void SetLastBlockHash(std::string LastBlockHash) { this->LastBlockHash = LastBlockHash; }
    void SetBestHashDiff(std::string BestHashDiff) { this->BestHashDiff = BestHashDiff; }
    void SetLastBlockTimeEnd(int LastBlockTimeEnd) { this->LastBlockTimeEnd = LastBlockTimeEnd; }
    void SetLastBLockMiner(std::string LastBLockMiner) { this->LastBLockMiner = LastBLockMiner; }
    void SetChecksCount(std::string ChecksCount) { this->ChecksCount = ChecksCount; }
    void SetLastBlockPoW(std::string LastBlockPoW) { this->LastBlockPoW = LastBlockPoW; }
    void SetLastBlockDiff(std::string LastBlockDiff) { this->LastBlockDiff = LastBlockDiff; }
    void SetSummary(std::string Summary) { this->Summary = Summary; }
    void SetGVTHash(std::string GVTHash) { this->GVTHash = GVTHash; }
    void SetNosoCFG(std::string NosoCFG) { this->NosoCFG = NosoCFG; }
    void SetPSOHash(std::string PSOHash) { this->PSOHash = PSOHash; }



    //Print Nodestatus String
    


};

size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* data) {
    data->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string GetNodePublicIP() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
    }

    return readBuffer;
};

class Node {
private:

    std::string NodeIP;
	int NodePort=8080;
	NodeStatusData NodeStatus;
    std::string MerkleTree;
    std::string ProgramVersion = "0.4.2";
    std::string Subversion = "Cb1";

public:
	Node(std::string NodeIP, int NodePort) : NodeIP(NodeIP), NodePort(NodePort) {}
	
    //Getters 
    std::string GetNodeIP() { return NodeIP; }
	int GetNodePort() { return NodePort; }
	//NodeStatusData GetNodeStatus() {
     //   return NodeStatus; }
    std::string GetMerkleTree() { return MerkleTree; }
	//Setters
    void CalculateMerkle()
        { 
        /*
        //NODESTATUS 1{Peers} 2{LastBlock} 3{Pendings} 4{Delta} 5{headers} 6{version} 7{UTCTime} 8{MNsHash}
//           9{MNscount} 10{LasBlockHash} 11{BestHashDiff} 12{LastBlockTimeEnd} 13{LBMiner}
//           14{ChecksCount} 15{LastBlockPoW} 16{LastBlockDiff} 17{summary} 18{GVTs} 19{nosoCFG}
//           20{PSOHash}    

1
[7:15 PM]
2{LastBlock}
5{headers}
8{MNsHash}
10{LasBlockHash}
17{summary}
18{GVTs}
19{nosoCFG}
        
        */
        
        //SetNodeStatus();
        std::string Consensus = std::to_string(NodeStatus.GetBlockNumber()) + NodeStatus.GetHeaders().substr(0,5) + NodeStatus.GetMNsHash().substr(0,5) + NodeStatus.GetLastBlockHash().substr(0,5) + NodeStatus.GetSummary().substr(0,5) + NodeStatus.GetGVTHash().substr(0,5) + NodeStatus.GetNosoCFG().substr(0,5);
       // std::cout << "String to Hash : " << Consensus << std::endl;
        std::string MerkleTree = calculateMD5(Consensus);
        this->MerkleTree = MerkleTree;
        //std:: cout << "String to Hash : " << Consensus << std::endl;    
       // std::cout << "MerkleTree: " << MerkleTree << std::endl;
		}
        
        
    


    void SetNodeStatus() { //NodeStatusData NodeStatus
        std::string NODESTATUS_COMMAND = "NODESTATUS\n";
        //std::cout << "Worked until SendString ToNode: ";
        std::string receivedStatus = SendStringToNode(NodeIP, NodePort, NODESTATUS_COMMAND);
        //std::cout << "Worked after SendString ToNode: ";
        if (receivedStatus == "NULL") {
        std::cout << "Error: Unable to receive NODESTATUS from Node: " << NodeIP << " Port: " << NodePort << std::endl;
            exit(EXIT_FAILURE); // Or handle the error in a different way
        }
        
        //Print receivedStatus string
        //std::cout << "Received Status: " << receivedStatus << std::endl;
        
        //NODESTATUS 51 149926 0 0 A6A78 0.4.2Cb1 1708194359 18ECB 259 55D3DC19805FDCE3DC3AE4BEE9040DA7 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1 1708194000 NpryectdevepmentfundsGE 0 54 FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF1 D4F62 9A623 0A373 27CBD
        std::istringstream NodeStatusIss(receivedStatus);

        // Read data directly from the received status string
        NodeStatusIss >> this->NodeStatus.NodeStatus;
        NodeStatusIss >> this->NodeStatus.Peers;
        NodeStatusIss >> this->NodeStatus.BlockNumber;
        NodeStatusIss >> this->NodeStatus.Pending;
        NodeStatusIss >> this->NodeStatus.Delta;
        NodeStatusIss >> this->NodeStatus.Headers;
        NodeStatusIss >> this->NodeStatus.Version;
        NodeStatusIss >> this->NodeStatus.UTCTIme;
        NodeStatusIss >> this->NodeStatus.MNsHash;
        NodeStatusIss >> this->NodeStatus.MNsCount;
        NodeStatusIss >> this->NodeStatus.LastBlockHash;
        NodeStatusIss >> this->NodeStatus.BestHashDiff;
        NodeStatusIss >> this->NodeStatus.LastBlockTimeEnd;
        NodeStatusIss >> this->NodeStatus.LastBLockMiner;
        NodeStatusIss >> this->NodeStatus.ChecksCount;
        NodeStatusIss >> this->NodeStatus.LastBlockPoW;
        NodeStatusIss >> this->NodeStatus.LastBlockDiff;
        NodeStatusIss >> this->NodeStatus.Summary;
        NodeStatusIss >> this->NodeStatus.GVTHash;
        NodeStatusIss >> this->NodeStatus.NosoCFG;
        NodeStatusIss >> this->NodeStatus.PSOHash;


    }

	
    void SetNodeIP(std::string NodeIP) { this->NodeIP = NodeIP; }
	void SetNodePort(int NodePort) { this->NodePort = NodePort; }

    //Print Node
    void PrintNode() {
		std::cout << "*Node IP: " << NodeIP << " *Node Port: " << NodePort << " *Node Merkle: " << MerkleTree << "NodeStatus " << NodeStatus.GetNodeStatus() << std::endl;
	}
    //Print Node In table format
    void PrintNodeTable() {
        
        std::cout << std::left << std::setw(15) << NodeIP << std::setw(10) << NodePort << std::setw(15) << MerkleTree << std::setw(20) << NodeStatus.GetNodeStatus() << std::endl;
        
    }
    void MyNodePresentation()
    {
        std::string presentation = "PSK " + GetNodePublicIP()+ " " + ProgramVersion + Subversion + " " + GetUTCTimeFromNTPServer();
        std::cout << presentation << std::endl;
        
        //std::string result = SendStringToNode(NodeIP, NodePort, "NOSOCFG\n");
        /*// 'PSK '+Address+' '+ProgramVersion+subversion+' '+UTCTimeStr);

-PSK             :Speecific String related to Pascal Source of Kreditz , this is a Legacy string
-Address         :Your públic Ip Address
-ProgramVersion  :Node Version
-UTCTimeStr      :Your UTC time in String Format.  */
    }
    

    std::string GetNodePublicIP() {
        CURL* curl;
        CURLcode res;
        std::string readBuffer;

        curl = curl_easy_init();
        if (curl) {
            curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
            res = curl_easy_perform(curl);
            curl_easy_cleanup(curl);
        }

        return readBuffer;
    }
};