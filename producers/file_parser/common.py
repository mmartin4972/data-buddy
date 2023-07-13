from abc import ABC, abstractmethod
import json
from typing import List, Dict, Any
import os
import sys

class IFileParser(ABC):
    @abstractmethod
    def __init__(self, name: str, use_ml: bool):
        pass

    @abstractmethod
    def parse(self, file_path: str) -> List[Dict[str, Any ]] :
        pass

    @abstractmethod
    def is_type(self, file_path: str, category: str) -> bool:
        pass

    @abstractmethod
    def get_name(self) -> str:
        pass

class Singleton:
    _instance = None

    def __new__(cls):
        if cls._instance is None:
            cls._instance = super().__new__(cls)
        return cls._instance

class PredefinedObjectsJSON(Singleton) :
    def __init__(self) :
        script_path = os.path.abspath(__file__)
        parent_directory = os.path.dirname(script_path)
        with open(os.path.join(parent_directory, '../../predefined_objects.json'), 'r') as f :
            self.predefined_json = json.load(f)

    def __getitem__(self, index):
        return self.predefined_json[index]
    
def datetime_to_rfc_3339(dt):
    return dt.isoformat("T") + "Z"